/*
  1.  Modifikovati aplikaciju za sinhronizaciju vremena tako da se
      aktuelno vreme i datum ispisuju na OLED displeju
*/

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <WiFi.h>
#include "time.h"

#include "Arduino.h"
#include "heltec.h"

const char* ssid     = "milin";
const char* password = "12345678";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;       //Offset u odnosu na UTC, offset je sat vremena
const int   daylightOffset_sec = 3600; 

void setup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  
  // Gasi WIFI da bi stedeo energiju
  WiFi.mode(WIFI_OFF); 

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void loop(){
  delay(1000);
  printLocalTime();
  printLocalTime_OLED();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time in printLocalTime()");
    return;
  }
  
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);

  time_t now;
  time(&now);
  Serial.print("UNIX TS: ");
  Serial.println(now);
  
  Serial.println();
}

void printLocalTime_OLED()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time in printLocalTime_OLED()");
      return;
    }

    // Create a string to hold the formatted time
    char formattedTimeFirstLine[22];
    char formattedTimeSecondLine[20];
    strftime(formattedTimeFirstLine, 22, "%A, %B %d %Y", &timeinfo);
    strftime(formattedTimeSecondLine, 20, "%H:%M:%S", &timeinfo);

    // Display the formatted time on OLED
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawStringMaxWidth(15, 20, 128, formattedTimeFirstLine);
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawStringMaxWidth(32, 40, 128, formattedTimeSecondLine);
    Heltec.display->display();

    Serial.println("Prikaz vremena na OLED ekranu");
}
