# ESP32 with MAX30102, DS18B20 & OLED

This project is a health monitoring system using ESP32, MAX30102 sensor, and DS18B20 temperature sensor. It measures heart rate, SpO2 (blood oxygen level), and body temperature, and displays the data on a 0.96 inch OLED screen.

## Features

- Heart rate measurement using MAX30102  
- SpO2 (blood oxygen level) measurement  
- Temperature measurement using DS18B20  
- I2C and OneWire communication  
- Real-time display on OLED (SSD1306 128x64)  
- Serial monitor debugging  
- Compact and real-time monitoring system  

## Components Required

- ESP32  
- MAX30102 Pulse Oximeter Sensor  
- DS18B20 Temperature Sensor  
- 4.7kΩ Resistor (pull-up for DS18B20)  
- 0.96" OLED Display (SSD1306, I2C)  
- Breadboard  
- Jumper wires  
- USB cable  

## Circuit Connections

### MAX30102 Sensor (I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

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

- The MAX30102 sensor detects heart rate and SpO2 using infrared and red light absorption.  
- The DS18B20 sensor measures temperature using OneWire protocol.  
- The ESP32 reads data from both sensors simultaneously.  
- The sensor generates heart rate (BPM), SpO2 (%), and temperature (°C).  
- All values are displayed on the OLED screen in real time.  
- Serial monitor shows detailed readings for debugging and monitoring.  

<h2 align="center">Project Images</h2>

<table align="center">
  <tr>
    <td align="left">
      <img src="" height="400"/>
    </td>
    <td align="right">
      <img src="" height="400"/>
    </td>
  </tr>
</table>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
