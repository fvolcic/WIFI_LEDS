#ifndef UninitializedAction
#define UninitializedAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"

class UninitilizedData : public LED_Action {

  private:

    rgb_color * ledData;
  public:

    UninitilizedData(int led_count, char * message) : LED_Action(led_count, false, true, false, true, message) {

      ledData = new rgb_color[led_count];
      
      }

   



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {

      for(int i = 0; i < led_count; ++i){
        colors[i] = ledData[i];   
      }

      
      this -> preparedForWrite = true;

    }



    void displayLEDs(rgb_color * colors) {
      this -> preparedForWrite = false;
       
       for(int i = 0; i < led_count; ++i){
        colors[i] = ledData[i];   
      }

      
      this -> preparedForWrite = true;
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
