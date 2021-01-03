#ifndef SubActionGaurd
#define SubActionGaurd
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"
#include "MQTTParser.h"
#include "SolidColorAction.cpp"

class SubAction : public LED_Action {

  private:

    int startIndex = 0;
    int numLEDS = 0; 
    LED_Action * action;

  public:

    //Message Length: 18
    //(8+FULL-MQTT-MESSAGE)GRAD{LED-STARTING-INDEX-4BYTES}{LED-ENDING-INDEX-4BYTES}{NUM-LEDS-4-BYTES}{FULL-MQTT-MESSAGE}
    //EX:
    //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
    SubAction(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message) {
      //colors = new rgb_color[led_count];

      MessageInformation msg(2, ":4:4:4N", message);

      char num[4];
      char * msgPtr;

      msgPtr = msg.getElt(0);
      num[0] = *msgPtr;
      num[1] = *(msgPtr + 1);
      num[2] = *(msgPtr + 2);
      num[3] = *(msgPtr + 3);
      startIndex = atoi(num);

      msgPtr = msg.getElt(1);
      num[0] = *msgPtr;
      num[1] = *(msgPtr + 1);
      num[2] = *(msgPtr + 2);
      num[3] = *(msgPtr + 3);
      int endIndex = atoi(num);

      /*
      msgPtr = msg.getElt(2);
      num[0] = *msgPtr;
      num[1] = *(msgPtr + 1);
      num[2] = *(msgPtr + 2);
      num[3] = *(msgPtr + 3);
      int numLEDS = atoi(num);
      */

      numLEDS = endIndex - startIndex; 
      
      MQTTParser parser(endIndex - startIndex);

      parser.parseMessage(msg.getElt(2));
      parser.createAction(numLEDS);

      if (parser.getActionAvailable())
        action = parser.getCreatedAction();
      else
        action = new SolidColor(numLEDS, "255000000");

    }


    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      action->setupLEDs(colors + startIndex); 
      this->preparedForWrite = true;
    }



    void displayLEDs(rgb_color * colors) {
      this->preparedForWrite = false;
      action->displayLEDs(colors + startIndex);
      this->preparedForWrite = true;
    }

    void alternateCoreActionSetup() {}
    void alternateCoreAction() {}
    //void alternateCoreMQTTActionSetup(MQTTClient &client){}
    //void alternateCoreMQTTAction(MQTTClient &client){}
    void deconstruct_displayLEDs() {

    }
    void deconstruct_alternateCoreAction() {}
    //void deconstruct_alternateCoreMQTTAction(MQTTClient &client){}

};

#endif
