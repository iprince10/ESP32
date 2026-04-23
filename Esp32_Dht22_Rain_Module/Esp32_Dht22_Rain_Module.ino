#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ── OLED ──────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define OLED_ADDRESS 0x3C

// ── DHT22 ─────────────────────────────────────
#define DHTPIN   4
#define DHTTYPE  DHT22

// ── Rain Sensor ───────────────────────────────
#define RAIN_AO  34   // Analog  → moisture level
#define RAIN_DO  35   // Digital → rain yes/no (threshold set by potentiometer)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

// ── Helper: rain intensity from analog value ──
String getRainLevel(int val) {
  // ESP32 ADC: 0 (wet) to 4095 (dry)
  if (val > 3500)       return "No Rain";
  else if (val > 2500)  return "Light";
  else if (val > 1500)  return "Moderate";
  else                  return "Heavy";
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(RAIN_DO, INPUT);

  // ── OLED init ──
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found!");
    while (true);
  }

  // ── DHT22 init ──
  dht.begin();
  delay(2000);              // let DHT22 stabilize
  dht.readTemperature();    // discard first noisy read
  dht.readHumidity();
  delay(500);

  // ── Splash screen ──
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.println("  Hii Prince :)");
  display.println("  System Ready");
  display.display();
  delay(2000);
}

void loop() {
  // ── Read DHT22 ──
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  // ── Read Rain Sensor ──
  int  rainAO      = analogRead(RAIN_AO);       // 0–4095
  bool isRaining   = !digitalRead(RAIN_DO);     // DO goes LOW when rain detected
  String rainLevel = getRainLevel(rainAO);

  // ── Serial Debug ──
  Serial.printf("Temp: %.2f C | Humidity: %.1f %%\n", temperature, humidity);
  Serial.printf("Rain AO: %d | DO: %s | Level: %s\n",
                rainAO, isRaining ? "RAIN" : "DRY", rainLevel.c_str());

  // ── DHT22 read failed ──
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 read failed!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 25);
    display.println("  Sensor Error!");
    display.display();
    delay(2000);
    return;
  }

  // ══ OLED Layout ══════════════════════════════
  display.clearDisplay();

  // ── Row 0: Title ──
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("Weather Station");

  // ── Divider ──
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);

  // ── Row 1: Temperature ──
  display.setTextSize(1);
  display.setCursor(0, 12);
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(40, 10);
  display.print(temperature, 1);
  display.print(" C");

  // ── Divider ──
  display.drawLine(0, 30, 127, 30, SSD1306_WHITE);

  // ── Row 2: Humidity ──
  display.setTextSize(1);
  display.setCursor(0, 33);
  display.print("Humi:");
  display.setTextSize(2);
  display.setCursor(40, 31);
  display.print(humidity, 0);
  display.print(" %");

  // ── Divider ──
  display.drawLine(0, 50, 127, 50, SSD1306_WHITE);

  // ── Row 3: Rain status ──
  display.setTextSize(1);
  display.setCursor(0, 54);
  display.print("Rain:");
  display.setCursor(35, 54);
  // Show level if raining, else "Clear"
  if (isRaining) {
    display.print(rainLevel);
  } else {
    display.print("Clear");
  }

  display.display();
  delay(2500); // DHT22 needs min 2s between reads
}