#include "config.h"
#include <ArduinoOTA.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONEWIREBUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

bool boot = true;

void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.hostname(MQTT_CLIENT_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

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
  int retries = 0;
  while (!client.connected()) {
    if(retries < 150) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(MQTT_CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
          Serial.println("connected");
          // Once connected, publish an announcement...
          if(boot == true) {
            client.publish(MQTT_CLIENT_NAME"/checkIn","Rebooted");
            boot = false;
          }
          if(boot == false) {
            client.publish(MQTT_CLIENT_NAME"/checkIn","Reconnected"); 
          }
        } else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          retries++;
          // Wait 5 seconds before retrying
          delay(5000);
        }
    }
    if(retries > 1500)
    {
    ESP.restart();
    }
  }
}

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();

  setup_wifi();

  client.setServer(MQTT_SERVER, 1883);

  ArduinoOTA.setHostname(MQTT_CLIENT_NAME);
  ArduinoOTA.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
    
  sensors.requestTemperatures(); 
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureF);
  Serial.println("ºF");

  // Process MQTT tasks
  String payload = String(temperatureF);
  client.publish(MQTT_CLIENT_NAME, (char *)payload.c_str());

  // Send every 20 seconds
  delay(20000);

  // Handle OTA
  ArduinoOTA.handle();
}
