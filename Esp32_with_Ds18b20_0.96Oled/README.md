# ESP32 Temperature Monitoring System (DS18B20 + OLED)

This project is a temperature monitoring system using ESP32 and DS18B20 sensor. It reads temperature data from the DS18B20 sensor and displays the output on a 0.96 inch OLED screen.

## Features

- Temperature measurement using DS18B20 sensor  
- Digital communication using OneWire protocol  
- Real-time display on OLED (SSD1306 128x64)  
- High accuracy temperature readings  
- Simple and beginner-friendly setup  

## Components Required

- ESP32-WROOM-32  
- DS18B20 Temperature Sensor  
- 4.7kΩ Resistor (pull-up)  
- 0.96" OLED Display (SSD1306, I2C)  
- Breadboard  
- Jumper wires  
- USB cable  

## Circuit Connections

### DS18B20 Sensor
- VCC  -> 3.3V  
- GND  -> GND  
- DATA -> GPIO4  
- 4.7kΩ resistor between VCC and DATA  

### OLED Display (SSD1306 I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

## Working

- The DS18B20 sensor measures temperature using digital OneWire communication.  
- The ESP32 reads temperature data from the sensor.  
- A pull-up resistor ensures stable communication on the data line.  
- The measured temperature is displayed on the OLED screen.  
- The display updates continuously with real-time values.  

## Working Image

<p align="left">
  <img src="" height="600"/>
</p>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
