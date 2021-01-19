#include <WiFi.h>
#include <MQTT.h>
#include <APA102.h>
#include "MQTTParser.h"
#include "ActionExecuter.h"
#include "FileManager.h"
#include "GlobalVariables.h"

#define LED_BUILTIN 2

int brightness = 8;

TaskHandle_t ledTask;
TaskHandle_t WiFiTask;


//PIN 14: Data Pin
//PIN 12: Clock Pin
APA102<DATAPIN, CLOCKPIN> ledStrip;

//These are the units that will manage the data for device
FileManager fileManager = FileManager();
ActionExecuter manager = ActionExecuter(ledStrip, /*LEDSTRIP_GLOBALS::led_count*/240);
MQTTParser parser = MQTTParser(LEDSTRIP_GLOBALS::led_count);


void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_GLOBALS::ssid, WIFI_GLOBALS::pass);

  MQTT_GLOBALS::client.begin("192.168.1.141", WIFI_GLOBALS::net);
  MQTT_GLOBALS::client.onMessage(messageReceived);

  pinMode(LED_BUILTIN, OUTPUT);

  //xTaskCreatePinnedToCore(updater, "OTA_Update", NULL, 
  
  connect();

  xTaskCreatePinnedToCore(runLEDs, "LED TASK", 10000, NULL, 0, &ledTask, 1); //Pin to core 1
  xTaskCreatePinnedToCore(runWiFiTasks, "WiFi TASK", 10000, NULL, 0, &WiFiTask, 0); //Pin to core 0
  
}

void loop(){vTaskDelete(NULL);}

//This is linked to the MQTT Client. This is called whenever a new message is received on a subscribed topic.
void messageReceived(String &topic, String &payload) {
  Serial.println("MESSAGE RECEIVED. PARSE STARTING");
  parser.parseMessage(payload);
  parser.manageLEDs(&manager);
}

//This connects the WiFi along with the MQTT server
void connect() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!MQTT_GLOBALS::client.connect("ESP2!", "try2", "try2")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  //Subscribe to the name of the device
  MQTT_GLOBALS::client.subscribe(MQTT_GLOBALS::MQTTChannel);
  digitalWrite(LED_BUILTIN, HIGH);
}

//This is for running the LEDs on a seperate core.
void runLEDs(void * parameter) {
  for (;;) {
    vTaskDelay(10);//To ensure the watchdog timer does not trigger
    manager.executeActions();
  }
}

void runWiFiTasks(void * parameter) {
  for (;;) {
    vTaskDelay(10);
    //Ensure the device is connected.
    if (!MQTT_GLOBALS::client.connected())
      connect();

    MQTT_GLOBALS::client.loop();
 
    vTaskDelay(10.0 / portTICK_PERIOD_MS); //This will delay the run WiFi task for 10 milliseconds.
    //This ensures that the task is delayed, but the core is not in use.

  }

}
