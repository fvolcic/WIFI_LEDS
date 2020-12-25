#ifndef MovingGradientColorAction
#define MovingGradientColorAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"

class MovingGradient : public LED_Action {

  private:

    //Allows for gradient to be calculated only once
    rgb_color * colors; 
    rgb_color c1;
    rgb_color c2; 
    int offset; 
    
  public: 

  //Message Length: 18
  //018GRAD{COLORS1-9BYTES}{COLOR2-9BYTES}
  //EX:018GRAD255000000000000255
  //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
   MovingGradient(int led_count, char * message) : LED_Action(led_count, true, false, false, true, message), c1(rgb_color(0,0,0)), c2(rgb_color(0,0,0)), offset(0){
    colors = new rgb_color[led_count]; 
     
    MessageInformation msg(2, "CC", message); 
     
    char * col1 = msg.getElt(0); 
    c1.red = msg.getColorValue('R', col1);   
    c1.green = msg.getColorValue('G', col1);  
    c1.blue = msg.getColorValue('B', col1);  
      
    col1 = msg.getElt(1); 
    c2.red = msg.getColorValue('R', col1);   
    c2.green = msg.getColorValue('G', col1);  
    c2.blue = msg.getColorValue('B', col1);  
    }
    
   

 
  
  //The setup function for the solid led colors
  void setupLEDs(rgb_color * colors){
/*
    Serial.print("Gradient Color 1: R:");
    Serial.print(c1.red);
    Serial.print(" G: ");
    Serial.print(c1.green);
    Serial.print(" B: ");
    Serial.print(c1.blue);
    Serial.println("");
     Serial.print("Gradient Color 2: R:");
    Serial.print(c2.red);
    Serial.print(" G: ");
    Serial.print(c2.green);
    Serial.print(" B: ");
    Serial.print(c2.blue);
    Serial.println("");
    */
    double colorDistance = 0;

    //Store the vector components
    double Vx = c1.red - c2.red; 
    double Vy = c1.green - c2.green; 
    double Vz = c1.blue - c2.blue; 
    
    //This retreives the spacial distance between the two colors. 
    colorDistance += c1.red - c2.red;
    colorDistance = sqrt(colorDistance*colorDistance + (c1.blue - c2.blue)* (c1.blue - c2.blue));
    colorDistance = sqrt(colorDistance*colorDistance + (c1.green - c2.green) * (c1.green - c2.green));

  //  Serial.print("The Distance Between the Colors is: ");
  //  Serial.println(colorDistance); 

    //The components are now in unit vector form
    Vx = Vx / colorDistance; 
    Vy = Vy / colorDistance; 
    Vz = Vz / colorDistance; 

  //  Serial.println("Printing Unit Vector Components");
  //  Serial.println(Vx);
  //  Serial.println(Vy);
  //  Serial.println(Vz);

    int half_led_count = led_count / 2; 
    
    //The size of each vector step through 3d space
    double stepSize = colorDistance / half_led_count; 

    Serial.print("Step Size: ");
    Serial.println(stepSize); 
    
    for(int i = 0; i < half_led_count; i ++){
      Serial.print(" - x color comp- ");
     // Serial.print((int)((double)c2.blue + i*(stepSize * Vz)));
      this->colors[i].red = (int)((double)c2.red + i*(stepSize * Vx));
      this->colors[i].green = (int)((double)c2.green + i*(stepSize * Vy)); 
      this->colors[i].blue = (int)((double)c2.blue + i*(stepSize * Vz)); 

      colors[i] = this->colors[i]; 
      this->colors[led_count - 1 - i] = this->colors[i];
      colors[led_count - 1 - i] = this->colors[i];
      /*
      Serial.print(" | R: ");
      Serial.print(colors[i].red);
      Serial.print(" G: ");
      Serial.print(colors[i].green);
      Serial.print(" B: ");
        Serial.print(colors[i].blue);
      */
    }

  //  Serial.println("");

   
    
    this->preparedForWrite = true;
  }


  int getCircularOffset(int index){
    if(index < led_count)
      return index; 
    else
      return index - led_count; 
  }
  
  void displayLEDs(rgb_color * colors){
   // this->preparedForWrite = false;
   // Serial.println("RUNNING GRADIENT DISPLAY");
   
    for(int i = 0; i < led_count; i ++){
        colors[i] = this->colors[this->getCircularOffset(i + offset)];
    }

    ++offset;
    if(offset == led_count)
      offset = 0;
      
    this->preparedForWrite = true;
  }

void alternateCoreActionSetup(){}
void alternateCoreAction(){}
//void alternateCoreMQTTActionSetup(MQTTClient &client){}
//void alternateCoreMQTTAction(MQTTClient &client){}
void deconstruct_displayLEDs(){
  delete colors; 
  }
void deconstruct_alternateCoreAction(){}
//void deconstruct_alternateCoreMQTTAction(MQTTClient &client){}
  
};
#endif
