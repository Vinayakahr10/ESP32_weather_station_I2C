#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";

#define offset 19800

String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "PUT_YOUR_API_KEY_HERE";

// PUT_YOUR_LOCATION_CREDENTIALS
String lat = "YOUR_LATITUDE";
String lon = "YOUR_LONGITUDE";

const int ledPin = 2;

// Initialize WiFi and NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", offset);

void setup()
{
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();

    pinMode(ledPin, OUTPUT);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("WiFi Connecting...");
        lcd.setCursor(1, 2);
        lcd.print("WiFi Connecting...");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(1, 2);
    lcd.print("WiFi Connected.");
    delay(2000);

    timeClient.begin();

    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("WEATHER STATION");
    lcd.setCursor(4, 2);
    lcd.print("SRINGERI, IN");
}

void loop()
{
    lcd.noCursor();
    lcd.noBlink();
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    String formattedTime = timeClient.getFormattedTime();

    // Display date and time
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon + 1;
    int currentYear = ptm->tm_year + 1900;
    String currentDate = String(monthDay) + "/" + String(currentMonth) + "/" + String(currentYear);

    // Determine day of the week
    String weekDays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    String dayName = weekDays[ptm->tm_wday];

    // Determine AM/PM
    int hours = ptm->tm_hour;
    String ampm = (hours >= 12) ? "PM" : "AM";
    if (hours > 12)
    {
        hours -= 12;
    }
    else if (hours == 0)
    {
        hours = 12;
    }
    String displayTime = String(hours) + formattedTime.substring(2);

    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(ledPin, HIGH);

        HTTPClient http;

        // Set HTTP Request Final URL with Location and API key information
        http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

        // Start connection and send HTTP Request
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            String JSON_Data = http.getString();
            Serial.println(JSON_Data);

            DynamicJsonDocument doc(2048);
            deserializeJson(doc, JSON_Data);
            JsonObject obj = doc.as<JsonObject>();

            const char *description = obj["weather"][0]["description"];
            const float temp = obj["main"]["temp"];
            const int humidity = obj["main"]["humidity"];

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("DATE: " + currentDate);
            lcd.setCursor(16, 0);
            lcd.print(dayName);

            lcd.setCursor(0, 1);
            lcd.print("TIME: " + displayTime);
            lcd.setCursor(14, 1);
            lcd.print(ampm);

            lcd.setCursor(0, 2);
            lcd.print(description);
            lcd.setCursor(0, 3);
            lcd.print("Temp:");
            lcd.print(temp);
            lcd.write(0xDF); // °C symbol
            lcd.print("C");
            lcd.print(" Hum:");
            lcd.print(humidity);
            lcd.print("%");
        }
        else
        {
            Serial.println("Error!");
            lcd.clear();
            lcd.print("Can't Get DATA!");
        }
        http.end();
    }
    delay(1000);
}