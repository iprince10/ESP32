## ESP32 with DHT11 and 0.96 Inch Oled Display

This project is designed to monitor temperature and humidity using the DHT11 sensor and an ESP32.

## Features

- **Real-Time Monitoring**: Continuously monitors temperature and humidity in real-time.
- **Arduino Compatible**: Compatible with Arduino IDE and utilizes the Arduino IoT Cloud library.

## Components

- **ESP32**: The main microcontroller.
- **DHT11 Sensor**: A temperature and humidity sensor that provides real-time environmental data.

## Setup Instructions

1. **Hardware Setup**:
    - Connect the DHT11 sensor to the ESP32 as follows:
      - Data pin to G4.
      - VCC to 3.3V.
      - GND to GND.
      
2. **Software Setup**:
    - Install the Arduino IDE.
    - Install the required libraries: `ArduinoIoTCloud`, `Arduino_ConnectionHandler`, and `DHT`.
    
3. **Upload the Code**:
    - Upload the code to your Esp32 using the Arduino IDE.

4. **Monitor Data**:
    - Use the 0.96 Oled to monitor temperature and humidity data in real-time.

## Usage

- Ensure your Esp32 is powered.
- Open the Serial Monitor in the Arduino IDE to see real-time data.

## Credits

- **Designed by**: Prince Jha
