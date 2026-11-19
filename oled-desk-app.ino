#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Secrets.h"

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
static const unsigned char PROGMEM image_paint_13_bits[] = {0x01, 0xff, 0xc0, 0x00, 0x1e, 0x00, 0x3c, 0x00, 0x30, 0x00, 0x06, 0x00, 0x60, 0x00, 0x03, 0x00, 0xc0, 0x00, 0x01, 0x00, 0x80, 0x00, 0x01, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x01, 0x80, 0xc0, 0x00, 0x01, 0x00, 0x40, 0x00, 0x03, 0x00, 0x40, 0x00, 0x06, 0x00, 0x20, 0x00, 0x1c, 0x00, 0x3f, 0xff, 0xf0, 0x00};
static const unsigned char PROGMEM image_paint_16_bits[] = {0x01, 0xf8, 0x00, 0x1e, 0x07, 0x00, 0x30, 0x00, 0xe0, 0x40, 0x00, 0x30, 0xc0, 0x00, 0x18, 0x80, 0x00, 0x08, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0xe0, 0x00, 0x0c, 0x3f, 0xff, 0xf0};

const char* ssid_name = SSID_NAME;
const char* ssid_password = SSID_PASSWD;
const String hackatime_api = HACKATIME_API;
const String hackatime_id = HACKATIME_ID;
const String time_api_key = TIME_API_KEY;
const String timezone = "Europe/Istanbul";

void drawScreen(void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setTextWrap(false);

  display.setCursor(5, 5);
  display.print(F("Time: 05:13"));

  display.drawRect(3,3,70,11, SH110X_WHITE);
  
  display.setCursor(5, 21);
  display.print(F("11 Days until Jan 1"));

  display.drawRect(3,19,116,11, SH110X_WHITE);

  display.setCursor(3, 40);
  display.print(F("Today Coded:"));

  display.setCursor(21, 53);
  display.print(F("8H 36M"));

  display.drawRect(1,36,73,27, SH110X_WHITE);

  display.setCursor(87,41);
  display.print(F("Date:"));

  display.setCursor(78,51);
  display.print(F("03/08/26"));
  display.drawRect(76,37,51,25, SH110X_WHITE);

  display.setCursor(106,6);
  display.print(F(">.<"));

  display.setCursor(81,6);
  display.print(F(">_<"));

  display.drawBitmap(78,2, image_paint_13_bits, 25,14, SH110X_WHITE);
  display.drawBitmap(104,1, image_paint_16_bits, 22,15, SH110X_WHITE);

  display.display();
}

void setup() {
  Serial.begin(115200);
  display.begin(0x3C, true);
  display.clearDisplay();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_name, ssid_password);
  Serial.println("connecting started");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting..");
  }
  Serial.println("connected!!");
  drawScreen();
  new_year_finder();
}

void loop() {

}

void time() {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://time-api.ereninki.hackclub.app/api/v1/time?timezone=" + timezone + "&api_key=" + time_api_key;
    http.begin(url);

    int httpResponseCode = http.GET();

    if(httpResponseCode == 200) {
      String payload = http.getString();

      JsonDocument doc;
      deserializeJson(doc, payload);

      String hour = doc["hour"];
      String minute  = doc["minute"];
    }
  }
}

void code_time() {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://hackatime.hackclub.com/api/hackatime/v1/users/" + hackatime_id + "/statusbar/today";
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + hackatime_api);

    int httpResponseCode = http.GET();

    if(httpResponseCode == 200) {
      String payload = http.getString();

      JsonDocument doc;
      deserializeJson(doc, payload);

      int hackatime_total_seconds = doc["data"]["grand_total"]["total_seconds"];
      int total_minutes = (hackatime_total_seconds % 3600) / 60;
      int total_hours = hackatime_total_seconds / 3600;
    }
  }
}

void new_year_finder() {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://time-api.ereninki.hackclub.app/api/v1/time?timezone=" + timezone + "&api_key=" + time_api_key;
    http.begin(url);

    int days_until_jan;
    int httpResponseCode = http.GET();
    
    if(httpResponseCode == 200) {
      String payload = http.getString();

      JsonDocument doc;
      deserializeJson(doc, payload);
      int day_of_the_year = doc["day_of_the_year"];
      if (day_of_the_year == 1) {
        days_until_jan = 0;
      } else {
        days_until_jan = 366 - day_of_the_year;
      }
    }
  }
}

void date() {

}