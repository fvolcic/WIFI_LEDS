#include <WiFi.h>
#include <MQTT.h>
#include <APA102.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ActionExecuter.h"
#include "LedAction.h"
#include "SolidColorAction.cpp"
#include "MQTTParser.cpp"
#include "FileManager.h"
#include "MessageReceiver.h"

#define LEDBUILTIN 2

const char ssid[] = "NETGEAR24";
const char pass[] = "littlecartoon561";


TaskHandle_t ledTask;

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
MQTTParser parser = MQTTParser();


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  client.begin("192.168.1.130", net);
  client.onMessage(messageReceived);

  //Setup the onboard led
  pinMode(LEDBUILTIN, OUTPUT); 
  digitalWrite(LEDBUILTIN, HIGH); 

  connect();


  
  
  //Pin the functionality that runs the LEDs onto the preferred core.
  //the loop function runs on core 0 by default. Wifi and other OS tasks
  //Also run on core 0 by default, so use core 1 if possible to spread out the work
  //from core to core
  xTaskCreatePinnedToCore(runLEDs, "LED TASK", 10000, NULL, 0, &ledTask, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Ensure the device is connected.
  if (!client.connected())
    connect();

  client.loop();
  delay(10); //For wifi stability
}

//This is linked to the MQTT Client. This is called whenever a new message is received on a subscribed topic.
void messageReceived(String &topic, String &payload) {
  Serial.println("MESSAGE RECEIVED. PARSE STARTING");
  parser.parseMessage(payload);
  parser.createAction(ledCount);

  if (parser.getActionAvailable())
    manager.add_LED_action(parser.getCreatedAction());

  if (parser.getKillAction())
    manager.remove_LED_action(parser.killActionIndex());

  if (parser.getEditAction())
    manager.edit_LED_action(parser.getEditIndex(), parser.getCreatedAction());

  if (parser.getClearActions())
    manager.clear_LED_actions();

  if (parser.getOTAUpdate_Available())
    update_firmware();
}


void connect() {
  digitalWrite(LEDBUILTIN, LOW); 
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("LED-strip", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  //Subscribe to the name of the device
  client.subscribe("/barLights");

  digitalWrite(LEDBUILTIN, HIGH); 
}

//This is for running the LEDs on a seperate core.
void runLEDs(void * parameter) {
  for (;;) {
    vTaskDelay(10);//To ensure the watchdog timer does not trigger
    manager.executeActions();
  }
}


//The reset function
void(* resetFunc) (void) = 0;

void update_firmware() {

  //End the current WiFi state
  WiFi.disconnect();

  //Begin the new WiFi process
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected\n");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  
  Serial.println("Beginning Update Process");
  Serial.println("Current Firmware Version: v1.0");
 

  Serial.println("To Exit Update Mode, Reboot the device");
  Serial.println("Will Automatically Reboot after 4 minutes"); 

  unsigned t = millis(); 
  unsigned duration_millis = 240000; 

  //While 20 seconds has not yet elapsed
  while (millis() < t + duration_millis) {
    ArduinoOTA.handle();
  }

  Serial.println("Update Failed - Rebooting Device"); 
  resetFunc(); 

}
