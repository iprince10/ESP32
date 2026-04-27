# ESP32 with BMP280 & OLED

This project is using ESP32 and BMP280 sensor. It measures temperature and atmospheric pressure and displays the data on a 0.96 inch OLED screen.

## Features

- Temperature measurement using BMP280  
- Atmospheric pressure measurement  
- I2C communication with ESP32  
- Real-time display on OLED (SSD1306 128x64)  
- Compact and efficient design  
- Suitable for weather and altitude-based applications  

## Components Required

- ESP32-WROOM-32  
- BMP280 Sensor Module  
- 0.96" OLED Display (SSD1306, I2C)  
- Breadboard  
- Jumper wires  
- USB cable  

## Circuit Connections

### BMP280 Sensor (I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

### OLED Display (SSD1306 I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

## Working

- The BMP280 sensor measures temperature and atmospheric pressure.  
- It communicates with ESP32 using I2C protocol.  
- The ESP32 reads sensor data continuously.  
- The measured values are displayed on the OLED screen.  
- The system can also be used for altitude estimation based on pressure changes.  

## Working Image

<p align="left">
  <img src="https://github.com/iprince10/ESP32-Wroom/blob/main/Esp32_with_Bmp280_0.96Oled/Esp32_Bmp280_Oled.jpeg" height="600"/>
</p>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
