#ifndef RANDOMIZER_H
#define RANDOMIZER_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class BrightnessRandomizer : public LED_Action {

  private:

 
    int brightnessMinBound = 50; 
    

  public:

    //Message Length: 1
    //00BGRD
    //EX:018GRAD255000000000000255
    //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
    BrightnessRandomizer(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message) {

     MessageInformation msg(1, ":3", message); 
     brightnessMinBound = atoi(msg.getElt(0)); 

    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      this->preparedForWrite = true;
      Serial.println("led setup complete");
    }


 
    void displayLEDs(rgb_color * colors) {

     
      this->preparedForWrite = false;

      for(int i = 0; i < led_count; ++i){

       double brightnessChange = random(brightnessMinBound,100); 

       colors[i].red = (double)colors[i].red * (brightnessChange / 100.0); 
       colors[i].green = (double)colors[i].green * (brightnessChange / 100.0); 
       colors[i].blue = (double)colors[i].blue * (brightnessChange / 100.0);  
        
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
