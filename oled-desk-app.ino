#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Secrets.h"
#include <WiFiClientSecure.h>

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
static const unsigned char PROGMEM image_paint_13_bits[] = {0x01, 0xff, 0xc0, 0x00, 0x1e, 0x00, 0x3c, 0x00, 0x30, 0x00, 0x06, 0x00, 0x60, 0x00, 0x03, 0x00, 0xc0, 0x00, 0x01, 0x00, 0x80, 0x00, 0x01, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x01, 0x80, 0xc0, 0x00, 0x01, 0x00, 0x40, 0x00, 0x03, 0x00, 0x40, 0x00, 0x06, 0x00, 0x20, 0x00, 0x1c, 0x00, 0x3f, 0xff, 0xf0, 0x00};
static const unsigned char PROGMEM image_paint_16_bits[] = {0x01, 0xf8, 0x00, 0x1e, 0x07, 0x00, 0x30, 0x00, 0xe0, 0x40, 0x00, 0x30, 0xc0, 0x00, 0x18, 0x80, 0x00, 0x08, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0x80, 0x00, 0x04, 0xe0, 0x00, 0x0c, 0x3f, 0xff, 0xf0};

const char* ssid_name = SSID_NAME;
const char* ssid_password = SSID_PASSWD;
const String hackatime_api = HACKATIME_API;
const String time_api_key = TIME_API_KEY;
const String timezone = "Europe/Istanbul";
String time_minute;
int januar_lenght;

struct Sumoftheinformations {
  char time_hour[8];
  char until_jan_1[8];
  char the_date[16];
  char daily_coding_time[16];
  int jan_length;
};

QueueHandle_t betweenscreen;

void drawScreen(String time, String until_jan, String code_time, String date, int january_width) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setTextWrap(false);

  display.setCursor(5, 5);
  display.print(F("Time: "));
  display.print(time);

  display.drawRect(3,3,70,11, SH110X_WHITE);
  
  display.setCursor(5, 21);
  display.print(until_jan);
  display.print(F(" Days until Jan 1"));

  display.drawRect(3,19,january_width,11, SH110X_WHITE);

  display.setCursor(3, 40);
  display.print(F("Today Coded:"));

  display.setCursor(21, 53);
  display.print(code_time);

  display.drawRect(1,36,73,27, SH110X_WHITE);

  display.setCursor(87,41);
  display.print(F("Date:"));

  display.setCursor(78,51);
  display.print(date);
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
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 25);
  display.print(F("Connecting to Wifi.."));
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_name, ssid_password);
  Serial.println("connecting started");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting..");
  }
  display.clearDisplay();
  display.setCursor(20, 25);
  display.print(F("Connected!!!"));
  display.display();

  Serial.println("connected!!");
  delay(250);

  betweenscreen = xQueueCreate(1, sizeof(Sumoftheinformations));

  xTaskCreatePinnedToCore(
    get_all_the_informations,
    "API",
    10240,
    NULL,
    1,
    NULL,
    0
  );
}

Sumoftheinformations the_datas;

void loop() {
  if(xQueueReceive(betweenscreen, &the_datas, portMAX_DELAY) == pdTRUE) {
    drawScreen(the_datas.time_hour, the_datas.until_jan_1, the_datas.daily_coding_time, the_datas.the_date, the_datas.jan_length);
  }
}

void get_all_the_informations(void *pvParameters) {
  for(;;) {
    if(WiFi.status() == WL_CONNECTED) {
      Sumoftheinformations the_big_sum;
      WiFiClientSecure client;
      client.setInsecure();
      HTTPClient http;
      String url = "https://oled-desk-api.ereninki.hackclub.app/get-the-informations?api_key=" + time_api_key + "&hackatime_api=" + hackatime_api + "&timezone=" + timezone;
      http.begin(client, url);

      int httpResponseCode = http.GET();

      if(httpResponseCode == 200) {
        String payload = http.getString();

        JsonDocument doc;
        deserializeJson(doc, payload);
        String hackatime_hours = doc["hackatime hours"];
        String hackatime_minutes = doc["hackatime minutes"];
        String hour = doc["hour"];
        String minute = doc["minute"];
        String month = doc["month"];
        String year = doc["year"];
        String day = doc["day"];
        String until_january = doc["until january"];
        
        String date = day + "-" + month + "-" + year;
        if (minute.toInt() < 10) {
          time_minute = hour + ":" + "0" + minute;
        } else {
          time_minute = hour + ":" + minute;
        }
      
        if(until_january.toInt() < 100) {
          januar_lenght = 116;
        } else {
          januar_lenght = 125;
        }

        String coding_time = hackatime_hours + "H " + hackatime_minutes + "M";
        date.toCharArray(the_big_sum.the_date, sizeof(the_big_sum.the_date));
        time_minute.toCharArray(the_big_sum.time_hour, sizeof(the_big_sum.time_hour));
        coding_time.toCharArray(the_big_sum.daily_coding_time, sizeof(the_big_sum.daily_coding_time));
        until_january.toCharArray(the_big_sum.until_jan_1, sizeof(the_big_sum.until_jan_1));
        the_big_sum.jan_length = januar_lenght;

        xQueueOverwrite(betweenscreen, &the_big_sum);
      }
      http.end();
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}