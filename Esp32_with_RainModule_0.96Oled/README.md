# ESP32 with Rain Module + OLED

This project is using ESP32 and a rain sensor module. It reads rain intensity using analog and digital outputs and displays the result on a 0.96 inch OLED screen.

## Features

- Rain detection using rain sensor module  
- Analog (AO) based rain intensity measurement  
- Digital (DO) based rain detection  
- Real-time display on OLED (SSD1306 128x64)  
- Rain classification (No Rain, Light, Moderate, Heavy)  
- Simple and beginner-friendly setup  

## Components Required

- ESP32-WROOM-32  
- Rain Sensor Module (AO + DO)  
- 0.96" OLED Display (SSD1306, I2C)  
- Breadboard  
- Jumper wires  
- USB cable  

## Circuit Connections

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

- The rain sensor detects water on its surface.  
- Analog output (AO) provides rain intensity values (0–4095):
  - Higher value indicates dry condition  
  - Lower value indicates wet condition  
- Digital output (DO) provides threshold-based detection:
  - LOW indicates rain detected  
  - HIGH indicates no rain  
- The ESP32 reads both AO and DO values.  
- Rain intensity is classified into:
  - No Rain  
  - Light  
  - Moderate  
  - Heavy  
- The result is displayed on the OLED screen in real time.  

## Working Image

<p align="left">
  <img src="https://github.com/iprince10/ESP32-Wroom/blob/main/Esp32_with_RainModule_0.96Oled/Esp32_RainModule_Oled.jpeg" height="600"/>
</p>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
