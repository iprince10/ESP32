#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define OLED_ADDRESS  0x3C

#define FINGER_THRESHOLD   50000
#define BPM_BUFFER_SIZE        6
#define SAMPLE_COUNT         100
#define WAVE_POINTS           64

// ── DS18B20 ───────────────────────────────────────
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
float tempF     = 0.0;
float tempC_val = 0.0;
bool  tempValid = false;
unsigned long lastTempRead = 0;
#define TEMP_INTERVAL 3000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

// ── SpO2 ──────────────────────────────────────────
uint32_t irBuffer[SAMPLE_COUNT];
uint32_t redBuffer[SAMPLE_COUNT];
int32_t  spo2      = 0;
int8_t   validSPO2 = 0;
int32_t  heartRateSPO2;
int8_t   validHR;
byte     spIdx     = 0;

// ── BPM ───────────────────────────────────────────
float bpmBuffer[BPM_BUFFER_SIZE];
byte  bpmIndex   = 0;
float bpmAvg     = 0;
long  irBaseline = 0;
bool  risingEdge = false;
long  lastBeatTime = 0;

// ── Waveform ──────────────────────────────────────
uint16_t waveBuffer[WAVE_POINTS];
byte     waveIndex = 0;

// ── Timing / Pages ────────────────────────────────
unsigned long lastDisplayUpd = 0;
unsigned long lastPageSwitch = 0;
bool          showTempPage   = false;

#define DISPLAY_INTERVAL  150
#define PAGE_INTERVAL    4000

// ─────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED fail"); while (1);
  }
   // ── Screen 1: Title ──
    display.clearDisplay();
    display.setTextColor(WHITE);

    display.setTextSize(2);
    display.setCursor(28, 2);   // 18 → 2  (moved up)
    display.print("Health");

    display.setCursor(20, 20);  // 36 → 20 (moved up)
    display.print("Monitor");

    display.setTextSize(1);
    display.setCursor(18, 54);  // unchanged
    display.print("By - Prince Jha");

    display.display();
    delay(3000);

  // ── DS18B20 ──
  tempSensor.begin();
  tempSensor.setResolution(11);
  tempSensor.setWaitForConversion(false);
  tempSensor.requestTemperatures();
  lastTempRead = millis();

  // ── MAX30102 ──
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    display.clearDisplay();
    display.setCursor(8, 28);
    display.print("MAX30102 not found!");
    display.display();
    while (1);
  }
  particleSensor.setup(0x1F, 4, 2, 100, 411, 4096);

  for (byte i = 0; i < BPM_BUFFER_SIZE; i++) bpmBuffer[i] = 0;
  for (int  i = 0; i < WAVE_POINTS;     i++) waveBuffer[i] = 0;

  // ── Screen 2: Initializing ──
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(20, 28);
    display.print("Initializing...");
    display.display();
    delay(1000);  // 2 seconds

}

// ─────────────────────────────────────────────────
float calcBPMAverage() {
  float sum = 0; byte cnt = 0;
  for (byte i = 0; i < BPM_BUFFER_SIZE; i++) {
    if (bpmBuffer[i] > 30 && bpmBuffer[i] < 200) { sum += bpmBuffer[i]; cnt++; }
  }
  return (cnt >= 2) ? (sum / cnt) : 0;
}

void drawWaveform(int yBase, int height) {
  uint16_t minV = 65535, maxV = 0;
  for (int i = 0; i < WAVE_POINTS; i++) {
    if (waveBuffer[i] < minV) minV = waveBuffer[i];
    if (waveBuffer[i] > maxV) maxV = waveBuffer[i];
  }
  if ((maxV - minV) < 50) return;
  for (int i = 1; i < WAVE_POINTS; i++) {
    int x0 = map(i-1, 0, WAVE_POINTS-1, 0, 127);
    int x1 = map(i,   0, WAVE_POINTS-1, 0, 127);
    int y0 = yBase - map(waveBuffer[(waveIndex+i-1) % WAVE_POINTS], minV, maxV, 0, height);
    int y1 = yBase - map(waveBuffer[(waveIndex+i)   % WAVE_POINTS], minV, maxV, 0, height);
    display.drawLine(x0, constrain(y0, yBase-height, yBase),
                     x1, constrain(y1, yBase-height, yBase), WHITE);
  }
}

void drawPageDots(bool tempPage) {
  display.drawCircle(60, 61, 2, tempPage   ? WHITE : WHITE);
  display.drawCircle(68, 61, 2, !tempPage  ? WHITE : WHITE);
  if (!tempPage) display.fillCircle(60, 61, 2, WHITE);
  else           display.fillCircle(68, 61, 2, WHITE);
}

// ── Page 1: BPM + SpO2 ───────────────────────────
void drawMainPage(bool fingerOn) {
  display.clearDisplay();

  // ── Title bar ──
  display.fillRect(0, 0, 128, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(8, 2);
  display.print("BPM");
  display.setCursor(72, 2);
  display.print("SpO2");
  display.setTextColor(WHITE);

  // ── Vertical divider ──
  display.drawFastVLine(63, 12, 48, WHITE);

  if (!fingerOn) {
    display.setTextSize(1);
    display.setCursor(2, 24);  display.print("Place");
    display.setCursor(2, 34);  display.print("finger");
    display.setCursor(2, 44);  display.print("on sensor");
    display.setCursor(68, 24); display.print("Place");
    display.setCursor(68, 34); display.print("finger");
    display.setCursor(68, 44); display.print("on sensor");
    drawPageDots(false);
    display.display();
    return;
  }

  // ── BPM (left side) ──
  bool bpmValid = (bpmAvg > 30 && bpmAvg < 200);
  display.setTextSize(2);
  int bpmInt = (int)bpmAvg;
  if (bpmValid) {
    display.setCursor(bpmInt >= 100 ? 1 : 8, 16);
    display.print(bpmInt);
  } else {
    display.setCursor(4, 16);
    display.print("--");
  }
  display.setTextSize(1);
  display.setCursor(4, 34);
  display.print("bpm");

  // BPM status
  display.setCursor(0, 46);
  if (bpmValid) {
    if      (bpmAvg < 60)  display.print("Low");
    else if (bpmAvg <= 100) display.print("Normal");
    else                   display.print("High");
  } else {
    display.print("Wait...");
  }

  // ── SpO2 (right side) ──
  bool sp2Valid = (validSPO2 && spo2 >= 80 && spo2 <= 100);
  display.setTextSize(2);
  if (sp2Valid) {
    display.setCursor(spo2 == 100 ? 65 : 72, 16);
    display.print(spo2);
  } else {
    display.setCursor(70, 16);
    display.print("--");
  }
  display.setTextSize(1);
  display.setCursor(72, 34);
  display.print("%");

  // SpO2 status
  display.setCursor(66, 46);
  if (sp2Valid) {
    if      (spo2 >= 95)  display.print("Normal");
    else if (spo2 >= 90)  display.print("Low");
    else                  display.print("Critic");
  } else {
    display.print("Wait...");
  }

  drawPageDots(false);
  display.display();
}

// ── Page 2: Temperature ───────────────────────────
void drawTempPage() {
  display.clearDisplay();

  // ── Title bar ──
  display.fillRect(0, 0, 128, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(22, 2);
  display.print("TEMPERATURE");
  display.setTextColor(WHITE);

  // ── Vertical divider ──
  display.drawFastVLine(63, 12, 44, WHITE);

  if (!tempValid) {
    display.setTextSize(1);
    display.setCursor(10, 30);
    display.print("Reading...");
    drawPageDots(true);
    display.display();
    return;
  }

  // ── °F (left) ──
  display.setTextSize(1);
  display.setCursor(4, 14);
  display.print("\xF8""F");

  display.setTextSize(2);
  // e.g. 98.6 → fits in left half
  display.setCursor(2, 24);
  display.print(tempF, 1);

  // ── °C (right) ──
  display.setTextSize(1);
  display.setCursor(68, 14);
  display.print("\xF8""C");

  display.setTextSize(2);
  display.setCursor(66, 24);
  display.print(tempC_val, 1);

  // ── Status bar ──
  display.drawFastHLine(0, 44, 128, WHITE);
  display.setTextSize(1);
  display.setCursor(0, 48);

  if      (tempF < 60)   display.print("Cold environment");
  else if (tempF < 75)   display.print("Comfortable room");
  else if (tempF < 85)   display.print("Warm environment");
  else if (tempF < 95)   display.print("Hot surroundings");

  // Likely body contact zone
  else if (tempF < 97)   display.print("Possible skin contact");
  else if (tempF <= 100) display.print("Body temp range");
  else if (tempF <= 103) display.print("High body temp");
  else                   display.print("Very high reading!");

  drawPageDots(true);
  display.display();
}

// ─────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // ══ DS18B20 — non-blocking ═══════════════════════
  if (now - lastTempRead >= TEMP_INTERVAL) {
    float rawC = tempSensor.getTempCByIndex(0);
    if (rawC != DEVICE_DISCONNECTED_C && rawC > -50.0 && rawC < 85.0) {
      tempC_val = rawC;
      tempF     = rawC * 9.0 / 5.0 + 32.0;
      tempValid = true;
    }
    tempSensor.requestTemperatures();
    lastTempRead = now;
  }

  // ══ MAX30102 — untouched ══════════════════════════
  while (!particleSensor.available()) particleSensor.check();
  uint32_t irVal  = particleSensor.getIR();
  uint32_t redVal = particleSensor.getRed();
  particleSensor.nextSample();

  bool fingerOn = (irVal > FINGER_THRESHOLD);

  waveBuffer[waveIndex] = (uint16_t)constrain(irVal >> 4, 0, 65535);
  waveIndex = (waveIndex + 1) % WAVE_POINTS;

  if (fingerOn) {
    const float ALPHA     = 0.95;
    const float THR_RATIO = 0.003;
    const long  MIN_GAP   = 400;
    const long  MAX_GAP   = 1500;

    if (irBaseline == 0) irBaseline = irVal;
    irBaseline = (long)(ALPHA * irBaseline + (1.0 - ALPHA) * irVal);

    long diff      = irVal - irBaseline;
    long threshold = (long)(irBaseline * THR_RATIO);

    if (diff > threshold) risingEdge = true;
    if (risingEdge && diff < 0) {
      risingEdge = false;
      long gap = now - lastBeatTime;
      if (gap > MIN_GAP && gap < MAX_GAP) {
        float bpm = 60000.0 / gap;
        bpmBuffer[bpmIndex] = bpm;
        bpmIndex = (bpmIndex + 1) % BPM_BUFFER_SIZE;
        bpmAvg = calcBPMAverage();
      }
      lastBeatTime = now;
    }

    redBuffer[spIdx] = redVal;
    irBuffer[spIdx]  = irVal;
    spIdx++;

    if (spIdx >= SAMPLE_COUNT) {
      maxim_heart_rate_and_oxygen_saturation(
        irBuffer, SAMPLE_COUNT, redBuffer,
        &spo2, &validSPO2, &heartRateSPO2, &validHR
      );
      for (byte i = 25; i < SAMPLE_COUNT; i++) {
        irBuffer[i-25]  = irBuffer[i];
        redBuffer[i-25] = redBuffer[i];
      }
      spIdx = SAMPLE_COUNT - 25;
    }

  } else {
    // Reset BPM only — keep SpO2 last reading on screen
    bpmAvg = 0; irBaseline = 0; risingEdge = false;
    for (byte i = 0; i < BPM_BUFFER_SIZE; i++) bpmBuffer[i] = 0;
  }

  // ── Page switch — always ticking ─────────────────
  if (now - lastPageSwitch >= PAGE_INTERVAL) {
    showTempPage   = !showTempPage;
    lastPageSwitch = now;
  }

  // ── Display ──────────────────────────────────────
  if (now - lastDisplayUpd >= DISPLAY_INTERVAL) {
    lastDisplayUpd = now;
    showTempPage ? drawTempPage() : drawMainPage(fingerOn);
  }
}