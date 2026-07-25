#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// =====================================================
// LCD
// =====================================================
LiquidCrystal_I2C lcd(0x27, 20, 4);

// =====================================================
// WiFi Credentials (Replace with your own)
// =====================================================
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// =====================================================
// OpenWeatherMap API
// Get your free API key:
// https://openweathermap.org/api
// =====================================================
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "YOUR_OPENWEATHER_API_KEY";

// Location Coordinates
// Example:
// Bengaluru:
// lat = "12.9716";
// lon = "77.5946";
String lat = "YOUR_LATITUDE";
String lon = "YOUR_LONGITUDE";

// =====================================================
// Time
// =====================================================
#define offset 19800   // UTC+5:30 (India)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", offset);

// =====================================================
// Status LED
// =====================================================
const int ledPin = 2;

// =====================================================
// Weather Update Interval
// =====================================================
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 600000; // 10 minutes

// =====================================================
// Weather Data
// =====================================================
String weatherDesc = "--";
float temperature = 0;
int humidity = 0;
String cityName = "Loading...";

// =====================================================
// Sanitize string for LCD
// =====================================================
String sanitizeForLCD(String input) {
  String result = "";

  for (int i = 0; i < input.length(); i++) {
    char c = input[i];

    if (c >= 32 && c <= 126)
      result += c;
  }

  return result;
}

// =====================================================
// WiFi Reconnect
// =====================================================
void checkWiFi() {

  if (WiFi.status() != WL_CONNECTED) {

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Reconnecting WiFi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("WiFi Connected");
    delay(1000);
  }
}

// =====================================================
// Get Weather Data
// =====================================================
void getWeatherData() {

  if (WiFi.status() == WL_CONNECTED) {

    digitalWrite(ledPin, HIGH);

    lcd.setCursor(0, 3);
    lcd.print("Updating...        ");

    HTTPClient http;

    String fullURL =
      URL +
      "lat=" + lat +
      "&lon=" + lon +
      "&units=metric&appid=" + ApiKey;

    http.begin(fullURL);

    int httpCode = http.GET();

    if (httpCode > 0) {

      String payload = http.getString();

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {

        weatherDesc =
            sanitizeForLCD(doc["weather"][0]["description"].as<String>());

        temperature = doc["main"]["temp"];
        humidity = doc["main"]["humidity"];

        cityName =
            sanitizeForLCD(doc["name"].as<String>());

      } else {

        weatherDesc = "JSON Error";
      }

    } else {

      weatherDesc = "HTTP Error";
    }

    http.end();

    digitalWrite(ledPin, LOW);
  }
}

// =====================================================
// Update Time
// =====================================================
void updateTime(String &dateStr,
                String &timeStr,
                String &dayStr,
                String &ampm) {

  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime((time_t *)&epochTime);

  int day = ptm->tm_mday;
  int month = ptm->tm_mon + 1;
  int year = ptm->tm_year + 1900;

  dateStr = String(day) + "/" +
            String(month) + "/" +
            String(year);

  String weekDays[7] =
  {
    "Sun","Mon","Tue",
    "Wed","Thu","Fri","Sat"
  };

  dayStr = weekDays[ptm->tm_wday];

  int hours = ptm->tm_hour;

  ampm = (hours >= 12) ? "PM" : "AM";

  hours %= 12;

  if (hours == 0)
    hours = 12;

  String minutesSeconds =
      timeClient.getFormattedTime().substring(2);

  timeStr = String(hours) + minutesSeconds;
}

// =====================================================
// Display Data
// =====================================================
void displayData() {

  String dateStr;
  String timeStr;
  String dayStr;
  String ampm;

  updateTime(dateStr, timeStr, dayStr, ampm);

  String day = dayStr.substring(0, 3);

  int maxCity = 20 - 1 - day.length();

  String city = cityName.substring(0, maxCity);

  // Line 1
  lcd.setCursor(0, 0);
  lcd.print("                    ");

  lcd.setCursor(0, 0);
  lcd.print(city);

  lcd.setCursor(20 - day.length(), 0);
  lcd.print(day);

  // Line 2
  lcd.setCursor(0, 1);
  lcd.print("                    ");

  lcd.setCursor(0, 1);
  lcd.print("DATE: " + dateStr);

  // Line 3
  lcd.setCursor(0, 2);
  lcd.print("                    ");

  lcd.setCursor(0, 2);
  lcd.print("TIME: " + timeStr + " " + ampm);

  // Line 4
  lcd.setCursor(0, 3);
  lcd.print("                    ");

  lcd.setCursor(0, 3);

  if (weatherDesc == "--") {

    lcd.print("Fetching data...");
    return;
  }

  lcd.print(weatherDesc.substring(0, 8));
  lcd.print(" ");

  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.print(humidity);
  lcd.print("%");
}

// =====================================================
// Setup
// =====================================================
void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(ledPin, OUTPUT);

  lcd.setCursor(0, 1);
  lcd.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  lcd.clear();

  lcd.setCursor(0, 1);
  lcd.print("WiFi Connected");

  delay(1000);

  timeClient.begin();

  // Initial Weather Fetch
  getWeatherData();
}

// =====================================================
// Loop
// =====================================================
void loop() {

  checkWiFi();

  if (millis() - lastWeatherUpdate > weatherInterval) {

    getWeatherData();

    lastWeatherUpdate = millis();
  }

  displayData();

  delay(1000);
}
