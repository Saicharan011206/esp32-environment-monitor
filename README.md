# esp32-environment-monitor
ESP32-based dual-sensor IoT system for real-time monitoring of temperature, humidity, CO₂, and air quality, with live data visualization on a web dashboard.


### What is ESP32?
The ESP32 is a powerful, low-cost microcontroller with integrated Wi-Fi and Bluetooth capabilities. It is widely used in IoT (Internet of Things) projects due to its versatility and high performance.
It features a dual-core processor, ample flash memory, and multiple GPIO pins, making it ideal for handling real-time sensor data and wireless communication simultaneously.

### Why ESP32 for This Project?

Fast processing speed: Handles real-time data collection and transmission efficiently.
Good onboard storage: Suitable for buffering sensor data and running complex code.
Multiple GPIO pins: Enables connection of multiple sensors (temperature, humidity, CO₂, air quality) simultaneously.
Built-in Wi-Fi and Bluetooth: Provides seamless IoT connectivity for real-time data transmission to a cloud or web-based frontend.
Low power consumption: Ideal for continuous environmental monitoring applications.

### Sensor Package:
#### DHT22:
Measures temperature and humidity with high accuracy.
Digital output sensor with a range of:
Temperature: -40°C to +80°C
Humidity: 0–100% RH

#### MQ135:

Detects air quality by measuring the concentration of gases such as CO₂, NH₃, benzene, alcohol, and smoke.
Produces an analog signal proportional to gas concentration.
Helps evaluate overall air pollution levels in the surrounding environment.

### Data Flow and IoT Integration:
The system continuously collects environmental data using the DHT22 and MQ135 sensors, processes it through the ESP32 microcontroller, and transmits it to a Firebase backend for real-time storage and visualization on a web dashboard.

#### Data Processing:
The ESP32 reads the sensor values, filters noise, and converts raw readings into meaningful units:
Temperature (°C)
Humidity (%)
CO₂ concentration (ppm)
Air Quality Index (AQI)
The data is formatted into a structured JSON format for transmission.

#### Data Transmission:
Using its built-in Wi-Fi, the ESP32 connects to the internet and sends sensor data to Firebase Realtime Database (or Firestore, depending on your setup).
Data is uploaded in real time using the Firebase ESP32 library.
Each reading is timestamped and stored in the database for historical tracking and live updates.

### Future Improvements:
Front-end dashboard can be used more efficiently to let a particular user to use mutliple devices and view all thier real-time data along with ability to set thresh-hold values and set notification alerts.

## Aurdino-Codes:
we started the project with the aurdino part so we went ahead with the aurdino coding.
Link to Branch:
https://github.com/Saicharan011206/esp32-environment-monitor/tree/arduino-codes
