/*
  2. Napisati aplikaciju koja: 
      1. Sinhronizuje vreme na ESP32 pomoću NTP 
      2. Omogućava korisniku unos vremena buđenja preko serijskog porta 
      3. Odlazi u Deep Sleep 
      4. Budi se u trenutku koji je korisnik zadao
*/

#include <WiFi.h>
#include <time.h>

#include "Arduino.h"
#include "heltec.h"

#define uS_TO_S_FACTOR 1000000  //faktor konverzije iz mikrosekundi u sekunde

const char* ssid     = "milin";
const char* password = "12345678";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;       //Offset u odnosu na UTC, offset je sat vremena
const int   daylightOffset_sec = 3600; 

time_t now;

struct tm t;
time_t when_to_turnON_ESP;

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
  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time in printLocalTime()");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  setWakeUpTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  
  // Gasi WIFI da bi stedeo energiju
  WiFi.mode(WIFI_OFF); 

  esp_sleep_enable_timer_wakeup((when_to_turnON_ESP - now) * uS_TO_S_FACTOR); // PODESAVAS KOLIKO DUGO CE SPAVATI 
  
  Serial.println("ESP32 podesen za spavanje na " + String(when_to_turnON_ESP - now) + " sekundi");

  //spavaj
  esp_deep_sleep_start();
}

void loop(){}

void setWakeUpTime()
{
  Serial.println("Print wakeUp time in format dd.mm.yy hh:mm:ss");
  
  while(Serial.available() == 0) {
    delay(100);
  }

  String input = Serial.readStringUntil('\n');
  Serial.println("You entered: ");
  Serial.println(input);
  Serial.println();

  t.tm_mday = input.substring(0, 2).toInt();
  t.tm_mon  = input.substring(3, 5).toInt() - 1;
  t.tm_year = input.substring(6, 10).toInt() - 1900;
  t.tm_hour = input.substring(11, 13).toInt() - 1;
  t.tm_min  = input.substring(14, 16).toInt();
  t.tm_sec  = input.substring(17, 19).toInt();
  when_to_turnON_ESP  = mktime(&t);

  char timeToString[24];
  sprintf(timeToString, "%d.%d.%d %d:%d:%d", t.tm_mday,t.tm_mon+1,t.tm_year + 1900,t.tm_hour,t.tm_min,t.tm_sec);
  Serial.println(timeToString);
  Serial.println();

  time(&now);
  
  Serial.println("UNIX TS: ");
  Serial.println(now);
  Serial.println();

  Serial.println("Second since the epoch :");
  Serial.println(when_to_turnON_ESP);
  Serial.println();

  Serial.println("Difference between UNIX TS and current time");
  Serial.println(when_to_turnON_ESP - now);
  Serial.println();
}