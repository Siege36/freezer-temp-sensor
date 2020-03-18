/*****************  START USER CONFIG SECTION *********************************/
// WIFI
#define WIFI_SSID                          ""
#define WIFI_PASSWORD                      ""

// MQTT Settings
#define MQTT_SERVER                   ""
#define MQTT_PORT                     0000
#define MQTT_USERNAME                 ""
#define MQTT_PASSWORD                 ""
#define MQTT_CLIENT_NAME              ""
#define MQTT_AVAILABILITY_TOPIC       MQTT_CLIENT_NAME"/available"

// OTA Settings
#define OTA_PORT                      8266
#define OTA_HOSTNAME                  MQTT_CLIENT_NAME

// Pin Settings
#define ONEWIREBUS                    4
