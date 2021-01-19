#include <MQTT.h>
#include <WiFi.h>

//These are variables for the clock and the data pin of the led strip
#ifndef APA102IOVALUES_H
#define APA102IOVALUES_H
#define DATAPIN 14
#define CLOCKPIN 12
#endif
//global variables needed for MQTT
namespace MQTT_GLOBALS{
  
  extern MQTTClient client; 

  extern String MQTTChannel;
  
}

//global variables needed for wifi
namespace WIFI_GLOBALS{

  extern WiFiClient net; 

  //Username and password for Wifi
  extern char * ssid; 
  extern char * pass; 
}

//global variables needed for the led strip
namespace LEDSTRIP_GLOBALS{
  extern const int led_count; 
}
