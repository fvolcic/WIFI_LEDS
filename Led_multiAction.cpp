/*

#ifndef LEDPointAction
#define LEDPointAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"

class LEDPoint : public LED_Action {

  private:

    struct pair{

      LED_Action * action; 
      int actionLength;
      
    };

    //Allows for gradient to be calculated only once
    rgb_color col;
    int dist;

    //This stores the multiple value pairs and their lengths
    pair * actionList; 
    int numActions; 
    
    

  public:

    //Message Length: 18
    //012PONT{COLORS1-9BYTES}{DIST-BETWEEN-POINTS-3BYTES}
    //EX:0
    //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}

    //This message is slightly more complex.
    //Format: 100MULTI{num actions}{actionMessage}{actionMessage}{actionMessage}{actionMessage}...{finalAcitonMessage}
    LEDPoint(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message), col(rgb_color(255, 255, 255)), dist(1) {

      char colDefiner[] = {'0', '0', '0'};
      int index = 0;

      char numActions[3]; 
      int index = 0; 

      numActions[0] = message[0];
      numActions[1] = message[1]; 
      numActions[2] = message[2]; 

      numActions = atoi(numActions); 

      char message[200]; 

      //reset the index
      index = 3; 

     while(index / 3 <= numActions){
       
       messag[index] = messgae[index];
       
    
      
     }

    };



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      this->preparedForWrite = true;
    }



    void displayLEDs(rgb_color * colors) {
      int index = 0;

      while (index < led_count) {
        colors[index] = col;
        index += dist;
      }

      this->preparedForWrite = true;
    }

    void alternateCoreActionSetup() {}
    void alternateCoreAction() {}
    void alternateCoreMQTTActionSetup(MQTTClient &client) {}
    void alternateCoreMQTTAction(MQTTClient &client) {}
    void deconstruct_displayLEDs() {}
    void deconstruct_alternateCoreAction() {}
    void deconstruct_alternateCoreMQTTAction(MQTTClient &client) {}

};

*/
//#endif
