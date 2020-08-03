#include <WiFi.h>
#include <MQTT.h>
#include <APA102.h>
#include "ActionExecuter.h"
#include "LedAction.h"
#include "SolidColorAction.cpp"
#include "MQTTParser.cpp"
#include "FileManager.h"
#include "MessageReceiver.h"

const char ssid[] = "NETGEAR24";
const char pass[] = "littlecartoon561";

int brightness = 8;

TaskHandle_t ledTask;

WiFiClient net;
MQTTClient client;

APA102<13,12> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 240;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];


//These are the units that will manage the data for device
FileManager fileManager = FileManager();
ActionExecuter manager = ActionExecuter(ledStrip, ledCount); 
MQTTParser parser = MQTTParser(); 


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  client.begin("192.168.1.251", net);
  client.onMessage(messageReceived);

  connect();

  xTaskCreatePinnedToCore(runLEDs, "LED TASK", 10000, NULL, 0, &ledTask, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
    
    //Ensure the device is connected. 
    if(!client.connected())
      connect();

    client.loop();
    delay(10); //For wifi stability
}



//This is linked to the MQTT Client. This is called whenever a new message is received on a subscribed topic. 
void messageReceived(String &topic, String &payload) {
 Serial.println("MESSAGE RECEIVED. PARSE STARTING");
 parser.parseMessage(payload); 
 parser.createAction(ledCount); 

 if(parser.getActionAvailable())
    manager.add_LED_action(parser.getCreatedAction());

  if(parser.getKillAction())
    manager.remove_LED_action(parser.killActionIndex());

  if(parser.getEditAction())
    manager.edit_LED_action(parser.getEditIndex(), parser.getCreatedAction());

  if(parser.getClearActions())
    manager.clear_LED_actions();

  if(parser.getBrightnessAvailable())
    manager.setBrightness(parser.getBrightness());
}


void connect() {
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
}

//This is for running the LEDs on a seperate core.
void runLEDs(void * parameter){
  for(;;){
  vTaskDelay(10);//To ensure the watchdog timer does not trigger
  manager.executeActions();
  }
}
