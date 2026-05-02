#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define OLED_ADDRESS  0x3C

#define FINGER_THRESHOLD   50000
#define BPM_BUFFER_SIZE        6
#define SAMPLE_COUNT         100
#define WAVE_POINTS           64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

// ── SpO2 ──────────────────────────────────────────
uint32_t irBuffer[SAMPLE_COUNT];
uint32_t redBuffer[SAMPLE_COUNT];
int32_t  spo2       = 0;
int8_t   validSPO2  = 0;
int32_t  heartRateSPO2;
int8_t   validHR;
byte     spIdx      = 0;
bool     bufferFull = false;

// ── BPM (custom peak detector) ────────────────────
float   bpmBuffer[BPM_BUFFER_SIZE];
byte    bpmIndex        = 0;
float   bpmAvg          = 0;
long    irBaseline      = 0;
bool    risingEdge      = false;
long    lastBeatTime    = 0;

// ── Waveform ──────────────────────────────────────
uint16_t waveBuffer[WAVE_POINTS];
byte     waveIndex = 0;

// ── Timing ────────────────────────────────────────
unsigned long lastDisplayUpd = 0;
unsigned long lastPageSwitch = 0;
bool          showSpO2Page   = false;

#define DISPLAY_INTERVAL  150
#define PAGE_INTERVAL    5000

// ─────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED fail"); while (1);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20, 28);
  display.print("Initializing...");
  display.display();

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    display.clearDisplay();
    display.setCursor(8, 28);
    display.print("MAX30102 not found!");
    display.display();
    while (1);
  }

  // Single clean config — no setPulseAmplitude overrides
  particleSensor.setup(0x1F, 4, 2, 100, 411, 4096);

  for (byte i = 0; i < BPM_BUFFER_SIZE; i++) bpmBuffer[i] = 0;
  for (int  i = 0; i < WAVE_POINTS;     i++) waveBuffer[i] = 0;

  display.clearDisplay();
  display.setCursor(14, 28);
  display.print("Place finger...");
  display.display();
}

// ── Rolling BPM average ───────────────────────────
float calcBPMAverage() {
  float sum = 0; byte cnt = 0;
  for (byte i = 0; i < BPM_BUFFER_SIZE; i++) {
    if (bpmBuffer[i] > 30 && bpmBuffer[i] < 200) {
      sum += bpmBuffer[i]; cnt++;
    }
  }
  return (cnt >= 2) ? (sum / cnt) : 0;
}

// ── Waveform draw ─────────────────────────────────
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

// ── BPM page ──────────────────────────────────────
void drawBPMPage(bool fingerOn) {
  display.clearDisplay();
  display.fillRect(0, 0, 128, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(30, 2);
  display.print("HEART RATE");
  display.setTextColor(WHITE);

  if (!fingerOn) {
    display.setCursor(14, 28); display.print("Place finger on");
    display.setCursor(24, 40); display.print("the sensor...");
    display.display(); return;
  }

  bool valid = (bpmAvg > 30 && bpmAvg < 200);
  display.setTextSize(3);
  int bpmInt = (int)bpmAvg;
  display.setCursor(valid ? (bpmInt >= 100 ? 8 : 22) : 8, 14);
  display.print(valid ? String(bpmInt) : "---");
  display.setTextSize(1);
  display.setCursor(96, 22);
  display.print("BPM");

  display.setCursor(0, 38);
  if (valid) {
    if      (bpmAvg < 60)  display.print("Low");
    else if (bpmAvg <= 100) display.print("Normal");
    else                   display.print("High");
  } else {
    display.print("Calculating...");
  }

  display.drawFastHLine(0, 44, 128, WHITE);
  drawWaveform(63, 17);

  display.fillCircle(60, 61, 2, WHITE);
  display.drawCircle(68, 61, 2, WHITE);
  display.display();
}

// ── SpO2 page ─────────────────────────────────────
void drawSpO2Page(bool fingerOn) {
  display.clearDisplay();
  display.fillRect(0, 0, 128, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(34, 2);
  display.print("SpO2 LEVEL");
  display.setTextColor(WHITE);

  if (!fingerOn) {
    display.setCursor(14, 28); display.print("Place finger on");
    display.setCursor(24, 40); display.print("the sensor...");
    display.display(); return;
  }

  bool sp2Valid = (validSPO2 && spo2 >= 80 && spo2 <= 100);
  display.setTextSize(3);
  display.setCursor(sp2Valid ? (spo2 == 100 ? 4 : 16) : 8, 14);
  display.print(sp2Valid ? String(spo2) : "---");
  display.setTextSize(2);
  display.setCursor(sp2Valid ? (spo2 == 100 ? 58 : 70) : 70, 18);
  display.print("%");

  display.drawRect(0, 40, 128, 8, WHITE);
  if (sp2Valid) {
    int barW = constrain(map(spo2, 80, 100, 0, 124), 0, 124);
    display.fillRect(2, 42, barW, 4, WHITE);
  }

  display.setTextSize(1);
  display.setCursor(0, 52);
  if (sp2Valid) {
    if      (spo2 >= 95)  display.print("Normal  O2 level");
    else if (spo2 >= 90)  display.print("Low  See a doctor");
    else                  display.print("Critical! Danger");
  } else {
    display.print("Calculating...");
  }

  display.drawCircle(60, 61, 2, WHITE);
  display.fillCircle(68, 61, 2, WHITE);
  display.display();
}

// ─────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  while (!particleSensor.available()) particleSensor.check();
  uint32_t irVal  = particleSensor.getIR();
  uint32_t redVal = particleSensor.getRed();
  particleSensor.nextSample();

  bool fingerOn = (irVal > FINGER_THRESHOLD);

  // ── Waveform buffer ──
  waveBuffer[waveIndex] = (uint16_t)constrain(irVal >> 4, 0, 65535);
  waveIndex = (waveIndex + 1) % WAVE_POINTS;

  if (fingerOn) {

    // ── Custom peak detector for BPM ──
    const float  ALPHA     = 0.95;
    const float  THR_RATIO = 0.003;
    const long   MIN_GAP   = 400;
    const long   MAX_GAP   = 1500;

    if (irBaseline == 0) irBaseline = irVal;
    irBaseline = (long)(ALPHA * irBaseline + (1.0 - ALPHA) * irVal);

    long diff      = irVal - irBaseline;
    long threshold = (long)(irBaseline * THR_RATIO);

    if (diff > threshold) {
      risingEdge = true;
    }
    if (risingEdge && diff < 0) {
      risingEdge = false;
      long gap = now - lastBeatTime;
      if (gap > MIN_GAP && gap < MAX_GAP) {
        float bpm = 60000.0 / gap;
        bpmBuffer[bpmIndex] = bpm;
        bpmIndex = (bpmIndex + 1) % BPM_BUFFER_SIZE;
        bpmAvg = calcBPMAverage();
        Serial.print("Beat! BPM=");
        Serial.print((int)bpm);
        Serial.print("  Avg=");
        Serial.println((int)bpmAvg);
      }
      lastBeatTime = now;
    }

    // ── SpO2 rolling buffer ──
    redBuffer[spIdx] = redVal;
    irBuffer[spIdx]  = irVal;
    spIdx++;

    // Recalculate once buffer is full, then keep rolling
    if (spIdx >= SAMPLE_COUNT) {
      maxim_heart_rate_and_oxygen_saturation(
        irBuffer, SAMPLE_COUNT, redBuffer,
        &spo2, &validSPO2,
        &heartRateSPO2, &validHR
      );
      // Shift buffer by 25 — keeps recent data, drops oldest
      for (byte i = 25; i < SAMPLE_COUNT; i++) {
        irBuffer[i - 25]  = irBuffer[i];
        redBuffer[i - 25] = redBuffer[i];
      }
      spIdx = SAMPLE_COUNT - 25;

      Serial.print("SpO2="); Serial.print(spo2);
      Serial.print(" valid="); Serial.println(validSPO2);
    }

  } else {
    // Finger removed
    bpmAvg = 0; irBaseline = 0; risingEdge = false;
    for (byte i = 0; i < BPM_BUFFER_SIZE; i++) bpmBuffer[i] = 0;
    spIdx = 0; spo2 = 0; validSPO2 = 0;
    showSpO2Page = false;
    lastPageSwitch = now;
  }

  // ── Page switch every 5s ──
  if (fingerOn && now - lastPageSwitch >= PAGE_INTERVAL) {
    showSpO2Page  = !showSpO2Page;
    lastPageSwitch = now;
  }

  // ── Display ──
  if (now - lastDisplayUpd >= DISPLAY_INTERVAL) {
    lastDisplayUpd = now;
    showSpO2Page ? drawSpO2Page(fingerOn) : drawBPMPage(fingerOn);
  }
}