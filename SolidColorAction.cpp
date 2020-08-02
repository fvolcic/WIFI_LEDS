#ifndef SolidColorAction
#define SolidColorAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"

class SolidColor : public LED_Action {

  private:

    rgb_color stripcolor; 
     char colDefiner[3];
  public: 

  SolidColor(int led_count, char * message) : LED_Action(led_count, false, true, false, true,message){
  
    Serial.println("Starting to run solid coloring");
   
    int index = 0; 
    for(char * msgPtr2 = message; *msgPtr2; ++msgPtr2){
      
      Serial.print(*msgPtr2);
     
       if(index < 3)
         colDefiner[index] = *msgPtr2; 

       if(index == 3) 
         stripcolor.red = atoi( colDefiner); 

       if(index >= 3 && index < 6)
         colDefiner[index - 3] = *msgPtr2; 

       if(index == 6) 
         stripcolor.green = atoi( colDefiner); 

       if(index >= 6)
         colDefiner[index - 6] = *msgPtr2; 

       if(index == 8) 
         stripcolor.blue = atoi( colDefiner);
       
       ++index; 
    }
    
   }; 

 
  
  //The setup function for the solid led colors
  void setupLEDs(rgb_color * colors){

    Serial.println("RUNNING LED SETUP");
   
    for(int i = 0; i < led_count; ++ i){
      colors[i].red = stripcolor.red;
      colors[i].blue = stripcolor.blue;
      colors[i].green = stripcolor.green;
    }

     this -> preparedForWrite = true; 
    
    Serial.println("Running Solid Color LED setup");
  }


  
  void displayLEDs(rgb_color * colors){
     for(int i = 0; i < led_count; ++ i){
      colors[i].red = stripcolor.red;
      colors[i].blue = stripcolor.blue;
      colors[i].green = stripcolor.green;
    }

   
    //Serial.println("RUNNING DISPLAY LEDS");
  }

void alternateCoreActionSetup(){}
void alternateCoreAction(){}
//void alternateCoreMQTTActionSetup(MQTTClient &client){}
//void alternateCoreMQTTAction(MQTTClient &client){}
void deconstruct_displayLEDs(){}
void deconstruct_alternateCoreAction(){}
//void deconstruct_alternateCoreMQTTAction(MQTTClient &client){}
  
};
#endif
