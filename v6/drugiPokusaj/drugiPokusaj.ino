#include <WiFi.h>
#include <PubSubClient.h>

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN            5

const char* ssid = "prestige";
const char* password = "mikro1231";
const char* mqtt_server = "ues.edu.rs";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int brightness = 0;

// MQTT topics
const char* outTopic = "outTopic";
const char* inTopic = "ESP32/milin_pwm";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Convert payload to integer
  int value = atoi((char*)payload);

  // Validate brightness value
  if (value >= 0 && value <= 255) {
    // Map the received value (0-255) to the PWM range (0-4095)
    int dutyCycle = map(value, 0, 255, 0, 4095);

    // Set brightness based on mapped duty cycle
    ledcWrite(LEDC_CHANNEL_0, dutyCycle);

    Serial.print("Brightness set to: ");
    Serial.println(value);
  } else {
    Serial.println("Invalid brightness value received.");
  }
}



void setup_wifi() {
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish(outTopic, "hello world");
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  // Initialize LED PWM
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    // Publish message every 2 seconds
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", now);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);
  }
}
