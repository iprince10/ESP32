# ESP32 with MAX30102 & OLED

This project is a health monitoring system using ESP32 and MAX30102 sensor. It measures heart rate and SpO2 (blood oxygen level) and displays the data on a 0.96 inch OLED screen.

## Features

- Heart rate measurement using MAX30102  
- SpO2 (blood oxygen level) measurement  
- I2C communication with ESP32  
- Real-time display on OLED (SSD1306 128x64)  
- Serial monitor debugging  
- Compact and real-time monitoring system  

## Components Required

- ESP32  
- MAX30102 Pulse Oximeter Sensor  
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

### OLED Display (SSD1306 I2C)
- VCC  -> 3.3V  
- GND  -> GND  
- SDA  -> GPIO21  
- SCL  -> GPIO22  

## Working

- The MAX30102 sensor detects heart rate and SpO2 using infrared and red light absorption.  
- The ESP32 reads sensor data via I2C communication.  
- The sensor generates both heart rate (BPM) and SpO2 (%) values.  
- The OLED displays real-time heart rate and oxygen level.  
- Serial monitor shows detailed readings for debugging and analysis.  

<h2 align="center">Project Images</h2>

<table align="center">
  <tr>
    <td align="left">
      <img src="https://github.com/iprince10/ESP32-Wroom/blob/main/Esp32_with_Max30102_Oled/Esp32_Max30102_Oled_HeartRate.jpeg" height="400"/>
    </td>
    <td align="right">
      <img src="https://github.com/iprince10/ESP32-Wroom/blob/main/Esp32_with_Max30102_Oled/Esp32_Max30102_Oled_Spo2.jpeg" height="400"/>
    </td>
  </tr>
</table>

## Owner and Licence
Made by **Prince Jha** . This project is free source and open for all :)
