#ifndef LEDPointAction
#define LEDPointAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"

class LEDPoint : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    rgb_color col;
    int dist;
    
  public: 

  //Message Length: 18
  //012PONT{COLORS1-9BYTES}{DIST-BETWEEN-POINTS-3BYTES}
  //EX:0
  //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
   LEDPoint(int led_count, char * message) : LED_Action(led_count, true, false, false, false,message),col(rgb_color(255,255,255)), dist(1){
    
    char colDefiner[] = {'0','0','0'};
    int index = 0; 
    
    for(char * msgPtr2 = message; *msgPtr2; ++msgPtr2){
       if(index < 3)
         colDefiner[index] = *msgPtr2; 

       if(index == 3) 
         col.red = atoi( colDefiner); 

       if(index >= 3 && index < 6)
         colDefiner[index - 3] = *msgPtr2; 

       if(index == 6) 
         col.green = atoi( colDefiner); 

       if(index >= 6 && index < 9)
         colDefiner[index - 6] = *msgPtr2; 

       if(index == 8) 
         col.blue = atoi( colDefiner);

        if(index >= 9 && index < 12)
         colDefiner[index - 9] = *msgPtr2; 

       if(index == 11) 
         dist = atoi(colDefiner);
      
       ++index; 
    }
    
   }; 

 
  
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
