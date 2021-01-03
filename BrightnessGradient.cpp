#ifndef BGradientColorAction
#define BGradientColorAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class BrightnessGradient : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    int offset;
    int mv_speed = 1;   
    int ticks = 0; 

  public:

    //Message Length: 1
    //00BGRD
    //EX:018GRAD255000000000000255
    //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
    BrightnessGradient(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message), offset(55) {

     MessageInformation msg(1, ":3", message); 
     mv_speed = atoi(msg.getElt(0)); 

    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      this->preparedForWrite = true;
      Serial.println("led setup complete");
    }


 
    void displayLEDs(rgb_color * colors) {

      if(ticks != mv_speed){
        ++ticks; 
      }else{
      offset = (offset + 1) % led_count;
      ticks = 0; 
      }
     

      this->preparedForWrite = false;
      int iter = offset;

      int half_strip = led_count; 
      
      for (int i = 0; i < half_strip; ++i) {


        int loc = iter % half_strip;

        double count = half_strip;

        int led_index = half_strip - i; 
        int led_index2 = half_strip + i; 
        
        colors[led_index].red = colors[led_index].red * ( 1 - (double)loc / count );
        colors[led_index].blue = colors[led_index].blue * ( 1 - (double)loc / count );
        colors[led_index].green = colors[led_index ].green * ( 1 - (double)loc / count );

     //   colors[led_index2].red = colors[led_index2].red * ( 1 - (double)loc / count );
     //   colors[led_index2].blue = colors[led_index2].blue * ( 1 - (double)loc / count );
     //   colors[led_index2].green = colors[led_index2].green * ( 1 - (double)loc / count );
        
        
        ++iter;
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
