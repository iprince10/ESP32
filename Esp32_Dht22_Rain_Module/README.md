# ESP32 Weather Station (DHT22 + Rain Sensor + OLED)

This project is a simple IoT-based weather station using ESP32. It measures temperature, humidity, and rain intensity, and displays the data on an OLED screen. The system also outputs data to the serial monitor for debugging.

## Features

- Temperature monitoring using DHT22  
- Humidity monitoring using DHT22  
- Rain detection using analog and digital outputs  
- Rain intensity classification (No Rain, Light, Moderate, Heavy)  
- OLED display output (SSD1306 128x64)  
- Serial monitor debugging  

## Components Required

- ESP32-WROOM-32  
- DHT22 Temperature and Humidity Sensor  
- Rain Sensor Module (AO + DO)  
- 0.96" OLED Display (SSD1306, I2C)  
- Breadboard  
- Jumper wires  
- USB cable  

## Circuit Connections

### OLED Display (SSD1306 I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

### DHT22 Sensor
- VCC  -> 3.3V  
- GND  -> GND  
- DATA -> GPIO4  

### Rain Sensor Module
- VCC -> 3.3V / 5V  
- GND -> GND  
- AO  -> GPIO34 (Analog input)  
- DO  -> GPIO35 (Digital input)  

## Working

- The DHT22 sensor reads temperature and humidity values.  
- The rain sensor provides:
  - Analog output (AO) for rain intensity (0–4095 range)  
  - Digital output (DO) for rain detection (LOW = rain detected)  
- The system classifies rain intensity into:
  - No Rain  
  - Light  
  - Moderate  
  - Heavy  
- All data is displayed on the OLED screen in a structured layout.  
- Serial monitor shows detailed debug information.  

## Working Image

<p align="left">
  <img src="https://github.com/iprince10/ESP32-Wroom/blob/main/Esp32_Dht22_Rain_Module/Esp32_Dht22_RainModule.jpeg" height="600"/>
</p>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
