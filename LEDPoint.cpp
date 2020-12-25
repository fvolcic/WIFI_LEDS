#ifndef LEDPointAction
#define LEDPointAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class LEDPoint : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    rgb_color col;
    int dist;
    
  public: 

  //Message Length: 12
  //012PONT{COLORS1-9BYTES}{DIST-BETWEEN-POINTS-3BYTES}
  //EX:0
  //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
   LEDPoint(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message), col(rgb_color(255,255,255)), dist(1){
    
    MessageInformation msg(2, "C:3", message);
     
    char * col1 = msg.getElt(0); 
    col.red = msg.getColorValue('R', col1);   
    col.green = msg.getColorValue('G', col1);  
    col.blue = msg.getColorValue('B', col1); 
    
    dist = atoi(msg.getElt(1)); 
    
   }

 
  
  //The setup function for the solid led colors
  void setupLEDs(rgb_color * colors){
    this->preparedForWrite = true;
  }
  

  
  void displayLEDs(rgb_color * colors){
    int index = 0; 
    
    while(index < led_count){
      colors[index] = col;
      index += dist; 
    }
    
    this->preparedForWrite = true;
  }

void alternateCoreActionSetup(){}
void alternateCoreAction(){}
void alternateCoreMQTTActionSetup(MQTTClient &client){}
void alternateCoreMQTTAction(MQTTClient &client){}
void deconstruct_displayLEDs(){}
void deconstruct_alternateCoreAction(){}
void deconstruct_alternateCoreMQTTAction(MQTTClient &client){}
  
};
#endif
