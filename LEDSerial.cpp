#ifndef LEDSerial_H
#define LEDSerial_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class LEDSerial : public LED_Action {

  private:


  void padAndPrintColor(rgb_color& col){
    printPaddedNum(col.red); 
    printPaddedNum(col.green);
    printPaddedNum(col.blue); 
    
  }


  void printPaddedNum(int val){
     
  if(val < 10){
    Serial.print("00"); 
    Serial.print(val);
  }else if(val < 100){
    Serial.print("0"); 
    Serial.print(val);
  }else{
    Serial.print(val); 
  }
    
  }

  public:

    //Message Length: 1
    //00BGRD
    //EX:018GRAD255000000000000255
    //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
    LEDSerial(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message) {
    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {

    }


 
    void displayLEDs(rgb_color * colors) {
      this->preparedForWrite = false; 

      Serial.print("LEDMSG"); 
      Serial.print(led_count);
      Serial.print(":"); 

      for(int i = 0; i < led_count; ++i){
        padAndPrintColor(colors[i]); 
      }
      Serial.println(); 
      Serial.flush(); 

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
