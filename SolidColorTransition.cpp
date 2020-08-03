#ifndef SolidTransitionAction
#define SolidTransitionAction
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"

class SolidTransition : public LED_Action {

  private:


    float hue2rgb(float p, float q,float t){
            
            
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < (float)1/(float)6) return p + (q - p) * 6 * t;
            if(t < (float)1/(float)2) return q;
            if(t < (float)2/(float)3) return p + (q - p) * ((float)2/(float)3 - t) * 6;
            return p;
        }

void  hslToRgb(float h,float s,float l, float * colorPtr){
    float r, g, b;

    if(s == 0){
        r = g = b = l; // achromatic
    }else{
        

        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        
        
        //println(h);
        //println(h + (float)1/(float)3);
        //println(h - (float)1/(float)3);
        r = hue2rgb(p, q, h + (float)1/(float)3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - (float)1/(float)3);
        
        //println("R: " + r + " G: " + g + " B: " + b);
    }

    float col[] = {r * 255, g * 255, b * 255};
    
   colorPtr[0] = col[0];
   colorPtr[1] = col[1];
   colorPtr[2] = col[2];
}

    rgb_color c1;
    rgb_color c2; 
    bool transitionComplete;

    int currentTransitionLEDs;
    
  public: 

  //Message Length: 18
  //018GRAD{COLORS1-9BYTES}{COLOR2-9BYTES}
  //EX:018GRAD255000000000000255
  //PASSED MESSAGE SHOULD BE OF FORM: {COLORS1-9BYTES}{COLOR2-9BYTES}
   SolidTransition(int led_count, char * message) : LED_Action(led_count, true, false, false, true,message), c1(rgb_color(0,0,0)), c2(rgb_color(0,0,0)), transitionComplete(false), currentTransitionLEDs(1){
    
    char colDefiner[3];
  
    int index = 0; 

    
    
    for(char * msgPtr2 = message; *msgPtr2; ++msgPtr2){
       if(index < 3)
         colDefiner[index] = *msgPtr2; 

       if(index == 3) 
         c1.red = atoi( colDefiner); 

       if(index >= 3 && index < 6)
         colDefiner[index - 3] = *msgPtr2; 

       if(index == 6) 
         c1.green = atoi( colDefiner); 

       if(index >= 6 && index < 9)
         colDefiner[index - 6] = *msgPtr2; 

       if(index == 8) 
         c1.blue = atoi( colDefiner);

      if(index >= 9 && index < 12) 
          colDefiner[index - 9] = *msgPtr2; 

      if(index == 11)
        c2.red = atoi(colDefiner); 

       if(index >= 12 && index < 15) 
          colDefiner[index - 12] = *msgPtr2; 

      if(index == 14)
        c2.green = atoi(colDefiner);

       if(index >= 15) 
          colDefiner[index - 15] = *msgPtr2; 

      if(index == 17)
        c2.blue = atoi(colDefiner);
        
       ++index; 
    }
    
   }; 

 
  
  //The setup function for the solid led colors
  void setupLEDs(rgb_color * colors){  

    //This makes all the colors initially the same color
    for(int i = 0; i < led_count; i++){ 
      
      colors[i] = c1;
      
    }
    
    this->preparedForWrite = true;
  }


  
  void displayLEDs(rgb_color * colors){
    this->preparedForWrite = false; 

    if(!transitionComplete){
      for(int i = 0; i < led_count; i ++){
        colors[i] = c1; 
      }
    }else{
      for(int i = 0; i < led_count; i ++){
        colors[i] = c2; 
      }
    }

    if(!transitionComplete){
      int ledPos = round(random(0,led_count - 1));

      for(int i = 0; i < currentTransitionLEDs; ++ i){
      //Serial.println("RANDOM POS: ");
      //Serial.println(ledPos);
      
      ledPos = (random(0,led_count - 1));
      
      colors[ledPos].red = 255;
      colors[ledPos].green = 255;
      colors[ledPos].blue = 255;

      }

      currentTransitionLEDs = (int)round((double)currentTransitionLEDs  * (double)1.1) + 1;
      //Serial.println( currentTransitionLEDs);
      
      if(currentTransitionLEDs >= led_count){
        currentTransitionLEDs = led_count; 
        transitionComplete = true; 
      }
    }

    if(transitionComplete){

      for(int i = 0; i < led_count; i++){ 
        colors[i] = c2;
        
      }
      

      //Make white
      if(currentTransitionLEDs == led_count){
        for(int i = 0; i < led_count; i++){ 
          colors[i].red = 255; 
          colors[i].blue = 255; 
          colors[i].green = 255;     
        }
        currentTransitionLEDs = (int)((double)currentTransitionLEDs/ (double) 1.1) - 1;
    }

    if(currentTransitionLEDs >= 5){
      
      int ledPos = round(random(0,led_count - 1));

      for(int i = 0; i < currentTransitionLEDs; ++ i){
     // Serial.println("RANDOM POS: ");
      //Serial.println(ledPos);
      
      ledPos = (random(0,led_count - 1));
      
      colors[ledPos].red = 255;
      colors[ledPos].green = 255;
      colors[ledPos].blue = 255;
      
    }

    //Serial.print("Before: ");
    //Serial.println(currentTransitionLEDs);
    currentTransitionLEDs = (int)((double)currentTransitionLEDs/ (double) 1.1) - 1;
    //Serial.println(currentTransitionLEDs);
  
    
    }else{

      float* randCol = new float[3]; 
      hslToRgb((float)random(0,100)/100.0, 1,0.5,randCol);
      
      currentTransitionLEDs = 1;
      transitionComplete = false; 
      c1.red = c2.red;
      c1.green = c2.green;
      c1.blue = c2.blue;
      c2.red = randCol[0];
      c2.green = randCol[1];
      c2.blue = randCol[2];
      
    }

    
    }

    
      
    this->preparedForWrite = true;
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
