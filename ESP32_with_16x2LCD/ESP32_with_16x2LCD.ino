#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C(address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  Wire.begin(21, 22); // SDA=GPIO21, SCL=GPIO22
  lcd.init();
  lcd.backlight(); // Turn on backlight
  
  lcd.setCursor(0, 0); // Column 0, Row 0
  lcd.print("Hii Prince :)");
  
  lcd.setCursor(0, 1); // Column 0, Row 1
  lcd.print("ESP32 + LCD OK");
}

void loop() {
}
