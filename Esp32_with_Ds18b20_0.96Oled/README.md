# ESP32 Soil Moisture Monitoring System

This project is a simple soil moisture monitoring system using ESP32. It reads moisture levels from a soil moisture sensor and displays analog (AO), digital (DO), and wet/dry status on the serial monitor.

## Features

- Soil moisture detection using analog and digital outputs  
- Real-time monitoring on serial monitor  
- Displays AO (moisture level), DO (threshold status), and condition (Wet/Dry)  
- Simple and beginner-friendly setup  
- Useful for smart irrigation systems  

## Components Required

- ESP32-WROOM-32  
- Soil Moisture Sensor (AO + DO)  
- Breadboard  
- Jumper wires  
- USB cable  

## Circuit Connections

### Soil Moisture Sensor
- VCC -> 3.3V / 5V  
- GND -> GND  
- AO  -> GPIO34 (Analog input)  
- DO  -> GPIO35 (Digital input)  

## Working

- The soil moisture sensor measures the water content in soil.  
- Analog output (AO) provides a value between 0–4095:
  - Lower value indicates wet soil  
  - Higher value indicates dry soil  
- Digital output (DO) provides threshold-based detection:
  - LOW indicates wet condition  
  - HIGH indicates dry condition  
- The ESP32 reads both AO and DO values.  
- The system prints:
  - AO value  
  - DO status  
  - Soil condition (Wet/Dry)  
- Output is continuously displayed on the serial monitor.  

## Working Image

<p align="left">
  <img src="" height="600"/>
</p>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
