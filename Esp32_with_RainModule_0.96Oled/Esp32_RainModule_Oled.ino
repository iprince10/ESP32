#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RAIN_AO  34   // Analog output from rain sensor
#define RAIN_DO  35   // Digital output from rain sensor

void setup() {
  Serial.begin(115200);
  pinMode(RAIN_DO, INPUT);

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Rain Monitor Ready");
  display.display();
  delay(2000);
}

void loop() {
  int analogVal  = analogRead(RAIN_AO);   // 0–4095 (12-bit ADC)
  int digitalVal = digitalRead(RAIN_DO);  // LOW = rain detected

  // Convert analog to percentage (higher raw = drier)
  int rainPercent = map(analogVal, 0, 4095, 100, 0);

  String status = (digitalVal == LOW) ? "RAIN DETECTED" : "No Rain";

  // Serial log
  Serial.printf("Analog: %d | Rain%%: %d | Status: %s\n",
                 analogVal, rainPercent, status.c_str());

  // OLED display
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("-- Rain Monitor --");

  display.setCursor(0, 20);
  display.print("Intensity: ");
  display.print(rainPercent);
  display.println("%");

  // Draw bar
  display.drawRect(0, 35, 128, 10, SSD1306_WHITE);
  display.fillRect(0, 35, rainPercent * 128 / 100, 10, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 52);
  display.println(status);

  display.display();
  delay(1000);
}