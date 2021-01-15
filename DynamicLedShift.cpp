#ifndef DynamicShiftAction_H
#define DynamicShiftAction_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

extern int getValue(char * startingIndex, int numLength); 

class DynamicShiftAction : public LED_Action {

  private:

    int shiftAmount = 1; 
    
    int shiftSpeed = 0;
    int currentShiftValue = 0; 

    rgb_color * copy_colors; 

    inline int getNextShiftIndex(int currentIndex){

      return (currentIndex + shiftAmount) % led_count; 

    }

    inline void colorSwap(rgb_color &c1, rgb_color &c2){
  
      rgb_color tmp = c1; 

      c1 = c2; 
      c2 = c1; 

    }
 
  public:

    //Message Format: {3 bytes - shift amount}{3 bytes - shift speed}
    DynamicShiftAction(int led_count, char * message) : LED_Action(led_count, false, true, false, false, message) {

      MessageInformation msg(1, ":3", message);
      shiftSpeed = getValue(msg.getElt(0), 3); 
      
      copy_colors = new rgb_color[led_count]; 
    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {

    }



    void displayLEDs(rgb_color * colors) {
     
    
      if(shiftSpeed != currentShiftValue){
        ++currentShiftValue; 
      }else{
          currentShiftValue = 0; 
          shiftAmount = (shiftAmount + 1) % led_count; 
      }
    
      
      preparedForWrite = false; 

      
      
      for(int i = 0; i < led_count; ++i){
          copy_colors[i] = colors[i];
      }

       for(int i = 0; i < led_count; ++i){
          colors[ (i + shiftAmount) % led_count ] = copy_colors[i];     
       }

      preparedForWrite = true; 
     
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
