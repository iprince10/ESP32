#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  // Start DS18B20
  sensors.begin();

  // Start OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  // Serial output (for debugging)
  Serial.print("Temp: ");
  Serial.print(tempF);
  Serial.println(" F");

  // OLED display
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 5);
  display.print("Body Temp:");

  display.setCursor(0, 20);
  display.print("C: ");
  display.print(tempC);
  display.print(" C");

  display.setCursor(0, 40);
  display.print("F: ");
  display.print(tempF);
  display.print(" F");

  display.display();

  delay(1000);
}
