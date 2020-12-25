#ifndef SolidColorAction
#define SolidColorAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class SolidColor : public LED_Action {

  private:

    rgb_color stripcolor;
 
  public:

    SolidColor(int led_count, char * message) : LED_Action(led_count, false, true, false, true, message) {

      MessageInformation msg(1, "C", message);

      char * col1 = msg.getElt(0);
      stripcolor.red = msg.getColorValue('R', col1);
      stripcolor.green = msg.getColorValue('G', col1);
      stripcolor.blue = msg.getColorValue('B', col1);

    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {

      Serial.println("RUNNING LED SETUP");

      for (int i = 0; i < led_count; ++ i) {
        colors[i].red = stripcolor.red;
        colors[i].blue = stripcolor.blue;
        colors[i].green = stripcolor.green;
      }

      this -> preparedForWrite = true;

      Serial.println("Running Solid Color LED setup");
    }



    void displayLEDs(rgb_color * colors) {
      for (int i = 0; i < led_count; ++ i) {
        colors[i].red = stripcolor.red;
        colors[i].blue = stripcolor.blue;
        colors[i].green = stripcolor.green;
      }
    }

    void alternateCoreActionSetup() {}
    void alternateCoreAction() {}
    //void alternateCoreMQTTActionSetup(MQTTClient &client){}
    //void alternateCoreMQTTAction(MQTTClient &client){}
    void deconstruct_displayLEDs() {}
    void deconstruct_alternateCoreAction() {}
    //void deconstruct_alternateCoreMQTTAction(MQTTClient &client){}

};
#endif
