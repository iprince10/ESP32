#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BMP280 bmp(&Wire);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  // Init BMP280 first
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
  display.setCursor(20, 20);
  display.println("BMP280 + OLED");
  display.setCursor(30, 35);
  display.println("Starting...");
  display.display();
  delay(2000);
}

void loop() {
  float temp     = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(1013.25);

  // Serial output
  Serial.println("--------------------");
  Serial.print("Temperature : "); Serial.print(temp, 2);     Serial.println(" C");
  Serial.print("Pressure    : "); Serial.print(pressure, 2); Serial.println(" hPa");
  Serial.print("Altitude    : "); Serial.print(altitude, 2); Serial.println(" m");

  // OLED output
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(15, 0);
  display.println("-- BMP280 Monitor --");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Temperature (large text)
  display.setTextSize(1);
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

  display.display();
  delay(2000);
}