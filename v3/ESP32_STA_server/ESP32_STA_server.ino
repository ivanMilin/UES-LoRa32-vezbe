/*
    WiFi rezimi rada:
    1. Station (STA) Mode
      • ESP32 se povezuje na postojeću WiFi mrežu

    2. Soft Access Point (AP) Mode
      • ESP32 kreira svoju privatnu WiFi mrežu u kojoj preuzima ulogu pristupne tačke 
      • Za razliku od klasičnog WiFi rutera, u ovom slučaju ESP32 kao pristupna tačka nema priključak na WAN (tj. pristup internetu) 
      • maksimalan broj uređaja koji mogu da se priključe na mrežu je ograničen na 5
*/

// Import required libraries
#include "ESPAsyncWebServer.h"
#include "WiFi.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "milin";
const char* password = "12345678";

// Set LED GPIO
const int ledPin = 25;

// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var)
{
  Serial.println(var);
  
  if(var == "STATE")
  {
    if(digitalRead(ledPin))
    {
      ledState = "ON";
    }
    else
    {
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}
 
void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  }
  );
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
