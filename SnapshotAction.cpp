#ifndef SNAPSHOTSHIFTACTION_H
#define SNAPSHOTSHIFTACTION_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class SnapshotShiftAction : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    rgb_color * colors;
    

  public:

   //MESSAGE
   //000SNAP
    SnapshotShiftAction(int led_count, char * message) : LED_Action(led_count, true, false, false, true, message) {
      colors = new rgb_color[led_count];
    }





    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {

      this->preparedForWrite = false;
      for (int i = 0; i < led_count; ++i) {
        this->colors[i] = colors[i];
      }
      this->preparedForWrite = true;
    }

    void displayLEDs(rgb_color * colors) {
      this->preparedForWrite = false;
      for (int i = 0; i < led_count; i ++) {
        colors[i] = this->colors[i];
      }
      this->preparedForWrite = true;
    }

    void alternateCoreActionSetup() {}
    void alternateCoreAction() {}
    //void alternateCoreMQTTActionSetup(MQTTClient &client){}
    //void alternateCoreMQTTAction(MQTTClient &client){}
    void deconstruct_displayLEDs() {
      delete[] colors;
    }
    void deconstruct_alternateCoreAction() {}
    //void deconstruct_alternateCoreMQTTAction(MQTTClient &client){}

};
#endif
