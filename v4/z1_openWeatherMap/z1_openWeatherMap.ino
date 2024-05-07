/*
    • Korišćenjem OpenWeatherMap API-ja preuzeti trenutne vremenske podatke za Novi Sad 
    • Prikazati na OLED displeju najvažnije vremenske parametre 
          (trenutna/minimalna/maksimalna temperatura, vlažnost vazduha, vazdušni pritisak, brzina i pravac vetra…) 
    • Omogućiti da se pritiskom tastera PRG smenjuje prikaz na displeju između trenutnih podataka i vremenske prognoze za sutrašnji dan
*/ 

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include <ArduinoJson.h>

#include "Arduino.h"
#include "heltec.h"

#define USE_SERIAL Serial

WiFiMulti wifiMulti;

float currentTemp_today;
float minTemp_today;
float maxTemp_today;
int humidity_today; 
int pressure_today;
float windSpeed_today;
int windDeg_today;

float currentTemp_tomorrow;
float minTemp_tomorrow;
float maxTemp_tomorrow;
int humidity_tomorrow; 
int pressure_tomorrow;
float windSpeed_tomorrow;
int windDeg_tomorrow;

int allowButton = 0, counter = 0;

void setup() 
{
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->clear();
    
    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) 
    {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    // MORAS DA PROMENIS SSID i PASSWORD !!!!
    wifiMulti.addAP("milin", "12345678");


    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) 
    {

      HTTPClient http;
      
      Serial.print("[HTTP] begin...\n");
      http.begin("https://api.openweathermap.org/data/2.5/forecast?q=Novi%20Sad&appid=eb08248aa3c9c2875df2031f9e0afddf&units=metric");
      
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      
      if(httpCode > 0) 
      {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        
        if(httpCode == HTTP_CODE_OK)
        {
          DynamicJsonDocument doc(16384);
          deserializeJson(doc, http.getString());

          currentTemp_today = doc["list"][0]["main"]["temp"];       // float
          minTemp_today     = doc["list"][0]["main"]["temp_min"];   // float
          maxTemp_today     = doc["list"][0]["main"]["temp_max"];   // float
          humidity_today      = doc["list"][0]["main"]["humidity"]; // int
          pressure_today      = doc["list"][0]["main"]["pressure"]; // int
          windSpeed_today   = doc["list"][0]["wind"]["speed"];      // float
          windDeg_today       = doc["list"][0]["wind"]["deg"];      // int

          currentTemp_tomorrow = doc["list"][8]["main"]["temp"];
          minTemp_tomorrow     = doc["list"][8]["main"]["temp_min"];
          maxTemp_tomorrow     = doc["list"][8]["main"]["temp_max"];
          humidity_tomorrow      = doc["list"][8]["main"]["humidity"]; 
          pressure_tomorrow      = doc["list"][8]["main"]["pressure"];
          windSpeed_tomorrow   = doc["list"][8]["wind"]["speed"];
          windDeg_tomorrow       = doc["list"][8]["wind"]["deg"];

          Serial.println();
          Serial.print("currentTemp_today : ");
          Serial.println(currentTemp_today);
          
          Serial.print("minTemp_today : ");
          Serial.println(minTemp_today);

          Serial.print("maxTemp_today : ");
          Serial.println(maxTemp_today);

          Serial.print("humidity_today : ");
          Serial.println(humidity_today);

          Serial.print("pressure_today : ");
          Serial.println(pressure_today);

          Serial.print("windSpeed_today : ");
          Serial.println(windSpeed_today);

          Serial.print("windDeg_today : ");
          Serial.println(windDeg_today);
          
          //==============================================
          
          Serial.println();
          Serial.print("currentTemp_tomorrow : ");
          Serial.println(currentTemp_tomorrow);
          
          Serial.print("minTemp_tomorrow : ");
          Serial.println(minTemp_tomorrow);

          Serial.print("maxTemp_tomorrow : ");
          Serial.println(maxTemp_tomorrow);

          Serial.print("humidity_tomorrow : ");
          Serial.println(humidity_tomorrow);

          Serial.print("pressure_tomorrow : ");
          Serial.println(pressure_tomorrow);

          Serial.print("windSpeed_tomorrow : ");
          Serial.println(windSpeed_tomorrow);

          Serial.print("windDeg_tomorrow : ");
          Serial.println(windDeg_tomorrow);
        }
      }
  
      else
      {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      
      //  Ovo "http.end()" zatvorice TCP socket na portu 80
      //  preuzeli smo sadrzaj, ne treba nam vise pristup serveru
      http.end();
    }
}

void loop() 
{
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_10);
    
    if (digitalRead(0) == LOW) {
        allowButton = !allowButton;
        delay(500); // debounce delay
    }
    
    if (allowButton == 0) 
    {
        //Serial.printf("Usao sam u if");
        Heltec.display->clear();
        Heltec.display->drawString(0, 0, "Current weather(Novi Sad)");
        Heltec.display->drawString(0, 10, "Current temp/max/min:"); 
        Heltec.display->drawString(0, 20, "    " + String(currentTemp_today) + "/" + String(maxTemp_today)+ "/"+ String(minTemp_today) +"°C");
        Heltec.display->drawString(0, 30, "Humidity: " + String(humidity_today) + "%");
        Heltec.display->drawString(0, 40, "Pressure: " + String(pressure_today) + "hPa");
        Heltec.display->drawString(0, 50, "Wind Speed: " + String(windSpeed_today) + "mps");
        //Heltec.display->drawString(0, 50, "Counter: " + String(counter));
    } 
    else 
    {
        //Serial.printf("Usao sam u else");
        Heltec.display->clear();
        Heltec.display->drawString(0, 0, "Tumorrow weather(Novi Sad)");
        Heltec.display->drawString(0, 10, "Current temp/max/min:");
        Heltec.display->drawString(0, 20, "    " + String(currentTemp_tomorrow) + "/" + String(maxTemp_tomorrow)+ "/"+ String(minTemp_tomorrow) +"°C");
        Heltec.display->drawString(0, 30, "Humidity: " + String(humidity_tomorrow) + "%");
        Heltec.display->drawString(0, 40, "Pressure: " + String(pressure_tomorrow) + "hPa");
        Heltec.display->drawString(0, 50, "Wind Speed: " + String(windSpeed_tomorrow) + "mps");
        //Heltec.display->drawString(0, 50, "Counter: " + String(counter));
    }
    
    Heltec.display->display();
    delay(1000); // delay to avoid rapid screen updates
}
