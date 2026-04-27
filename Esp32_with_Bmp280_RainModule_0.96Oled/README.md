# ESP32 with BMP280 + Rain Module + OLED

This project is a weather monitoring system using ESP32, BMP280 sensor, and a rain sensor module. It measures temperature, atmospheric pressure, and rain intensity, and displays the data on a 0.96 inch OLED screen.

## Features

- Temperature measurement using BMP280  
- Atmospheric pressure measurement  
- Rain detection using analog and digital outputs  
- Rain intensity classification (No Rain, Light, Moderate, Heavy)  
- Real-time display on OLED (SSD1306 128x64)  
- Serial monitor debugging  

## Components Required

- ESP32-WROOM-32  
- BMP280 Sensor Module  
- Rain Sensor Module (AO + DO)  
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

### Rain Sensor Module
- VCC -> 3.3V / 5V  
- GND -> GND  
- AO  -> GPIO34 (Analog input)  
- DO  -> GPIO35 (Digital input)  

### OLED Display (SSD1306 I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

## Working

- The BMP280 sensor measures temperature and atmospheric pressure using I2C communication.  
- The rain sensor provides:
  - Analog output (AO) for rain intensity (0–4095 range)  
  - Digital output (DO) for rain detection (LOW = rain detected)  
- The ESP32 reads data from both sensors.  
- Rain intensity is classified into:
  - No Rain  
  - Light  
  - Moderate  
  - Heavy  
- All values are displayed on the OLED screen in a structured format.  
- Serial monitor prints temperature, pressure, AO value, DO status, and rain level for debugging.  

## Working Image

<p align="left">
  <img src="" height="600"/>
</p>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
