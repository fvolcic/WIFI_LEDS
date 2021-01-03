#include <WiFi.h>
#include <MQTT.h>
#include <APA102.h>
#include "MQTTParser.h"
#include "ActionExecuter.h"
#include "LedAction.h"
#include "SolidColorAction.cpp"
#include "FileManager.h"


#define LED_BUILTIN 2

int brightness = 8;

char * ssid = "NETGEAR24";
char * pass = "littlecartoon561"; 

TaskHandle_t ledTask;
TaskHandle_t WiFiTask;

WiFiClient net;
MQTTClient client;

APA102<13, 12> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 240;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];


//These are the units that will manage the data for device
FileManager fileManager = FileManager();
ActionExecuter manager = ActionExecuter(ledStrip, ledCount);
MQTTParser parser = MQTTParser(ledCount);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  client.begin("192.168.1.131", net);
  client.onMessage(messageReceived);

  pinMode(LED_BUILTIN, OUTPUT);

  //xTaskCreatePinnedToCore(updater, "OTA_Update", NULL, 
  
  connect();

  xTaskCreatePinnedToCore(runLEDs, "LED TASK", 10000, NULL, 0, &ledTask, 0); //Pin to core 1
  xTaskCreatePinnedToCore(runWiFiTasks, "WiFi TASK", 10000, NULL, 0, &WiFiTask, 0); //Pin to core 0
  
}

  void loop(){}

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
  while (!client.connect("192.168.1.207", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  //Subscribe to the name of the device
  client.subscribe("/barLights");
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
    if (!client.connected())
      connect();

    client.loop();
 
    vTaskDelay(10.0 / portTICK_PERIOD_MS); //This will delay the run WiFi task for 10 milliseconds.
    //This ensures that the task is delayed, but the core is not in use.

  }

}
