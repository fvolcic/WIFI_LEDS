#ifndef MIRRORACTION_H
#define MIRRORACTION_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class MirrorAction : public LED_Action {

  private:



  public:

    //MessageLength: 0
    //MESSAGE: 000MIRR
    MirrorAction(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message) {

      // MessageInformation msg(1, ":3", message);
      // mv_speed = atoi(msg.getElt(0));

    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      this->preparedForWrite = true;
      Serial.println("Mirroring");
    }



    void displayLEDs(rgb_color * colors) {

      this->preparedForWrite = false;
  
      rgb_color tmp;

      int leftIndex = 0; 
      int rightIndex = led_count - 1; 

      while(leftIndex < rightIndex){
        tmp = colors[leftIndex]; 
        colors[leftIndex] = colors[rightIndex]; 
        colors[rightIndex] = tmp; 
        ++leftIndex; 
        --rightIndex; 
      }
        

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
