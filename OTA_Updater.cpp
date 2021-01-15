#include <WiFi.h>
#include <ArduinoOTA.h>

#include "GlobalVariables.h"

#ifndef UPDAGAURD
#define UPDAGAURD

//The reset function
void(* resetFunc) (void) = 0;

void update_firmware(void * p) {

  //End the current WiFi state
  WiFi.disconnect();

  //Begin the new WiFi process
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_GLOBALS::ssid, WIFI_GLOBALS::pass);

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
    vTaskDelay(10);
    ArduinoOTA.handle();
  }

  Serial.println("Update Failed - Rebooting Device");
  resetFunc();
  
}

#endif
