/*
    WiFi rezimi rada:
    1. Station (STA) Mode
      • ESP32 se povezuje na postojeću WiFi mrežu

    2. Soft Access Point (AP) Mode
      • ESP32 kreira svoju privatnu WiFi mrežu u kojoj preuzima ulogu pristupne tačke 
      • Za razliku od klasičnog WiFi rutera, u ovom slučaju ESP32 kao pristupna tačka nema priključak na WAN (tj. pristup internetu) 
      • maksimalan broj uređaja koji mogu da se priključe na mrežu je ograničen na 5
*/

#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "LoRaWiFI";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80); // 80 je default port za HTTP server

uint8_t LEDpin = 25;  // PIN koji se koristi za kontrolu LED diode
bool LEDstatus = LOW; 

void setup() 
{
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);
  
  server.begin();   // Ovako se pokrece server
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  digitalWrite(LEDpin, LEDstatus);
}

void handle_OnConnect() {
  LEDstatus = LOW;
  Serial.println("GPIO25 Status: OFF");
  server.send(200, "text/html", SendHTML(LEDstatus)); //200 znaci da je sve OK - success
}

void handle_ledon() 
{
  LEDstatus = HIGH;
  Serial.println("GPIO25 Status: ON");
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() 
{
  LEDstatus = LOW;
  Serial.println("GPIO25 Status: OFF");
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t ledstat)
{
  String html = "<!DOCTYPE html> <html>\n";
  html +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html +="<title>LED Control</title>\n";
  html +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  html +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  html +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  html +=".button-on {background-color: #3498db;}\n";
  html +=".button-on:active {background-color: #2980b9;}\n";
  html +=".button-off {background-color: #34495e;}\n";
  html +=".button-off:active {background-color: #2c3e50;}\n";
  html +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  html +="</style>\n";
  html +="</head>\n";
  html +="<body>\n";
  html +="<h1>ESP32 Web Server</h1>\n";
  html +="<h3>Using Access Point(AP) Mode</h3>\n";
  
  if(ledstat)
    html +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  else
    html +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";

  html +="</body>\n";
  html +="</html>\n";
  
  Serial.print(html);
  return html;
}
