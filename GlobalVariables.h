#include <MQTT.h>
#include <WiFi.h>

namespace MQTT_GLOBALS{
  
  extern MQTTClient client; 
}

namespace WIFI_GLOBALS{

  extern WiFiClient net; 

  //Username and password for Wifi
  extern char * ssid; 
  extern char * pass; 
}

namespace LEDSTRIP_GLOBALS{
  extern const int led_count; 
}
