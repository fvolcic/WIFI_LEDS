#ifndef SHIFTACTION_H
#define SHIFTACTION_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class ShiftAction : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    rgb_color * colors;
    int offset;

  public:

   //MESSAGE
   //000SHFT
    ShiftAction(int led_count, char * message) : LED_Action(led_count, true, false, false, true, message), offset(0) {
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


    int getCircularOffset(int index) {
      if (index < led_count)
        return index;
      else
        return index - led_count;
    }

    void displayLEDs(rgb_color * colors) {
      this->preparedForWrite = false;
      // Serial.println("RUNNING GRADIENT DISPLAY");

      for (int i = 0; i < led_count; i ++) {
        colors[i] = this->colors[this->getCircularOffset(i + offset)];
      }

      ++offset;
      if (offset == led_count)
        offset = 0;

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
