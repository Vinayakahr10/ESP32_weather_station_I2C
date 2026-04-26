# Weather Station using ESP32 and LCD

This project is a simple weather station that uses an ESP32 microcontroller, a 20x4 LCD display, and the OpenWeatherMap API to display real-time weather information, date, and time. The system fetches weather data over WiFi and displays it in a user-friendly manner.

---

## Features

- Connects to the internet via WiFi.
- Fetches real-time weather data from the OpenWeatherMap API.
- Displays:
  - Date and time (synchronized via NTP).
  - Current weather conditions (e.g., temperature, humidity, description).
- LED indicator for WiFi connection status.
- User-friendly 20x4 LCD output.

---

## Hardware Requirements

1. **ESP32 Development Board**
2. **20x4 I2C LCD Display**
3. **LED** and **Resistor** (optional, for WiFi status indication)
4. **Connecting Wires**
5. **Power Source** (USB cable or battery)

---

## Software Requirements

1. **Arduino IDE** with ESP32 Board Manager installed
2. **Required Libraries**:
   - `WiFi`
   - `HTTPClient`
   - `LiquidCrystal_I2C`
   - `ArduinoJson`
   - `NTPClient`

---

## Circuit Diagram

[Insert a clear circuit diagram or link to an image here.]

---

## Setup Instructions

### 1. Hardware Connections

- Connect the 20x4 I2C LCD to the ESP32 (SDA and SCL pins).
- Optionally connect an LED with a resistor to a GPIO pin for WiFi status indication.

### 2. Configure the Code

1. Open the `weather_station.ino` file in the Arduino IDE.
2. Replace the following placeholders in the code:
   - `YOUR_SSID`: Your WiFi network name.
   - `YOUR_PASSWORD`: Your WiFi password.
   - `PUT_YOUR_API_KEY_HERE`: Your OpenWeatherMap API key.
   - `YOUR_LATITUDE` and `YOUR_LONGITUDE`: Your location coordinates.
3. Install the required libraries in the Arduino IDE:
   - Go to **Tools > Manage Libraries** and install:
     - `LiquidCrystal_I2C`
     - `ArduinoJson`
     - `NTPClient`

### 3. Upload the Code

1. Connect your ESP32 to your computer via USB.
2. Select the correct board and COM port in **Tools**.
3. Upload the sketch to your ESP32.

---

## How to Use

1. Power on the ESP32.
2. The system will connect to WiFi and fetch the current date, time, and weather.
3. The LCD will display:
   - Current date and time.
   - Weather conditions (temperature, humidity, and description).
4. If the system fails to fetch data, it will display an error message.

---

## Example Output

### LCD Display Example:

---

## Troubleshooting

1. **WiFi Connection Issues**:
   - Ensure the SSID and password are correct.
   - Check if the ESP32 is within range of the WiFi network.
2. **Weather Data Not Displayed**:
   - Verify your OpenWeatherMap API key.
   - Ensure the API endpoint and location credentials are correct.
3. **LCD Not Working**:
   - Check the I2C address of your LCD (default: `0x27`).
   - Verify the wiring connections.

---

## License

This project is open-source and licensed under the MIT License. Feel free to modify and distribute.

---

## Acknowledgements

- **OpenWeatherMap API**: For providing real-time weather data.
- **Arduino Libraries**: For simplifying hardware-software interfacing.

---

## Contributing

Contributions are welcome! If you have ideas for improvement, feel free to fork the repository and submit a pull request.

---

## Contact

For any issues or questions, feel free to contact me at `vinayakahr10@gmail.com`.

