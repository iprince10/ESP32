#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

#define RAIN_AO  34
#define RAIN_DO  35

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BMP280 bmp(&Wire);

// Pages to cycle through on OLED
int currentPage = 0;
unsigned long lastSwitch = 0;
const int PAGE_INTERVAL = 3000; // switch page every 3 seconds

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RAIN_DO, INPUT);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  // Init BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not found!");
    while (true);
  }
  Serial.println("BMP280 OK");

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found!");
    while (true);
  }
  Serial.println("OLED OK");

  // Splash screen
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 15);
  display.println("Weather Station");
  display.setCursor(25, 30);
  display.println("BMP280 + Rain");
  display.setCursor(30, 45);
  display.println("Starting...");
  display.display();
  delay(2000);
}

void displayBMPPage(float temp, float pressure, float altitude) {
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("-- BMP280 Data --");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Temperature large
  display.setCursor(0, 14);
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(40, 12);
  display.print(temp, 1);
  display.setTextSize(1);
  display.setCursor(110, 14);
  display.print("C");

  display.drawLine(0, 30, 128, 30, SSD1306_WHITE);

  // Pressure
  display.setTextSize(1);
  display.setCursor(0, 34);
  display.print("Pres: ");
  display.print(pressure, 1);
  display.println(" hPa");

  // Altitude
  display.setCursor(0, 46);
  display.print("Alt:  ");
  display.print(altitude, 1);
  display.println(" m");

  // Page indicator
  display.setCursor(110, 56);
  display.print("1/2");

  display.display();
}

void displayRainPage(int rainPercent, bool isRaining) {
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("-- Rain Sensor --");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Status large
  display.setTextSize(2);
  display.setCursor(0, 14);
  if (isRaining) {
    display.println("RAINING!");
  } else {
    display.println("No Rain");
  }

  display.drawLine(0, 34, 128, 34, SSD1306_WHITE);

  // Intensity bar
  display.setTextSize(1);
  display.setCursor(0, 38);
  display.print("Intensity: ");
  display.print(rainPercent);
  display.println("%");

  display.drawRect(0, 50, 128, 8, SSD1306_WHITE);
  display.fillRect(0, 50, rainPercent * 128 / 100, 8, SSD1306_WHITE);

  // Page indicator
  display.setCursor(110, 56);
  display.print("2/2");

  display.display();
}

void loop() {
  // Read BMP280
  float temp     = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(1013.25);

  // Read Rain Sensor
  int analogVal   = analogRead(RAIN_AO);
  int digitalVal  = digitalRead(RAIN_DO);
  int rainPercent = map(analogVal, 0, 4095, 100, 0);
  bool isRaining  = (digitalVal == LOW);

  // Serial output
  Serial.println("=============================");
  Serial.print("Temperature : "); Serial.print(temp, 2);      Serial.println(" C");
  Serial.print("Pressure    : "); Serial.print(pressure, 2);  Serial.println(" hPa");
  Serial.print("Altitude    : "); Serial.print(altitude, 2);  Serial.println(" m");
  Serial.print("Rain Raw    : "); Serial.println(analogVal);
  Serial.print("Rain%%      : "); Serial.print(rainPercent);  Serial.println("%");
  Serial.print("Raining     : "); Serial.println(isRaining ? "YES" : "NO");

  // Cycle OLED pages every PAGE_INTERVAL ms
  if (millis() - lastSwitch >= PAGE_INTERVAL) {
    currentPage = (currentPage + 1) % 2;
    lastSwitch = millis();
  }

  if (currentPage == 0) {
    displayBMPPage(temp, pressure, altitude);
  } else {
    displayRainPage(rainPercent, isRaining);
  }

  delay(1000);
}