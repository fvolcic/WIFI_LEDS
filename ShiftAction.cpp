#ifndef LEDPointAction
#define LEDPointAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class LEDShift : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    int shift_speed = 0;
    int shift_amount = 0;

  public:

    //Message Length: 12
    //012SHFT{SHIFT-AMOUNT-6BYTES}{SHIFT-SPEED-6BYTES}
    //EX:012SHFT000002000004
    //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
    LEDShift(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message) {

      MessageInformation msg(2, ":6:6", message);

      char 6_byte_num[6];

      char * index = msg.getElt(0);
      6_byte_num[0] = index[0];
      6_byte_num[1] = index[1];
      6_byte_num[2] = index[2];
      6_byte_num[3] = index[3];
      6_byte_num[4] = index[4];
      6_byte_num[5] = index[5];
      shift_amount = atoi(6_byte_num);
      
      index = msg.getElt(1); 
      shift_speed = atoi(index); 
    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      this->preparedForWrite = true;
    }



    void displayLEDs(rgb_color * colors) {
      
      while (index < led_count) {
        colors[index] = col;
        index += dist;
      }
      
    }

    void alternateCoreActionSetup() {}
    void alternateCoreAction() {}
    void alternateCoreMQTTActionSetup(MQTTClient &client) {}
    void alternateCoreMQTTAction(MQTTClient &client) {}
    void deconstruct_displayLEDs() {}
    void deconstruct_alternateCoreAction() {}
    void deconstruct_alternateCoreMQTTAction(MQTTClient &client) {}

};
#endif
