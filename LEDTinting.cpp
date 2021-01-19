#ifndef TINTRANDOMIZER_H
#define TINTRANDOMIZER_H
#include <APA102.h>
#include <MQTT.h>
#include "LedAction.h"
#include "MessageInformation.h"
#include "RandomGaussian.h"

enum DistType { Nonrandom, Uniform, ChiSquare};

class TintRandomizer : public LED_Action {

  private:


    rgb_color tintColor; //The tint color

    //random bounds
    int alpha_lower;
    int alpha_upper;

    //degrees of freedom
    int DOF = 1; 

    bool randomTint = false;
    bool chi_square = false;

    DistType distribution;

    double alpha_final;

    rgb_color * rgb_colors;
    void (TintRandomizer::*tintingFunction)(); //a tinting function pointer


    inline void getTintValue(rgb_color * initial, rgb_color &tint, double alpha) {

      initial->red = (tint.red - initial->red) * alpha + initial->red;
      initial->green = (tint.green - initial->green) * alpha + initial->green;
      initial->blue = (tint.blue - initial->blue) * alpha + initial->blue;

    }

    void alpha_zero_tinting() {


      for (int i = 0; i < led_count; ++i) {
        getTintValue(rgb_colors + i, tintColor, alpha_final);
      }

    }

    void alpha_nonzero_tinting() {
      for (int i = 0; i < led_count; ++i) {
        double a = random(alpha_lower, alpha_upper) / 100.0;
        getTintValue(rgb_colors + i, tintColor, a);
      }

    }

    void chi_tint() {
      for (int i = 0; i < led_count; ++i) {
        getTintValue(rgb_colors + i, tintColor, generateRandomChiSquare(DOF));
      }
    }

  public:

    //Message Length: 1
    //000TINT255255255050003
    //EX:
    //PASSED MESSAGE SHOULD BE OF FORM: {TINT COLOR-9BYTES}{TINT-LOWER}{TINT-UPPER}{CHI-SQUARE-BOOL}{DEGREESOFFREEDOM}
    TintRandomizer(int led_count, char * message) : LED_Action(led_count, true, false, false, false, message) {

      MessageInformation msg(1, "C:3:3N:2", message);

      char * tintIndex = msg.getElt(0);

      tintColor.red = msg.getColorValue('R', tintIndex);
      tintColor.green = msg.getColorValue('G', tintIndex);
      tintColor.blue = msg.getColorValue('B', tintIndex);

      alpha_lower = getValue(msg.getElt(1), 3);
      alpha_upper = getValue(msg.getElt(2), 3);


      if (*msg.getElt(3) == 'C') {
        distribution = DistType::ChiSquare;
        DOF = getValue(msg.getElt(4), 2); 
        Serial.println("Chi");
        Serial.print("DOF: "); 
        Serial.println(DOF); 
        
      }
      else if (alpha_lower == alpha_upper) {
        Serial.println("Nonrandom");
        distribution = DistType::Nonrandom;
        alpha_final = alpha_lower / 100.0;
      }
       else {
        distribution = DistType::Uniform;
        Serial.println("Uniform");
      }

    }



    //The setup function for the solid led colors
    void setupLEDs(rgb_color * colors) {
      this->preparedForWrite = true;
      rgb_colors = colors;
      Serial.println("led setup complete");
    }



    void displayLEDs(rgb_color * colors) {
      this->preparedForWrite = false;
      switch (distribution) {
        case (DistType::Uniform): {
            alpha_nonzero_tinting();
        } case (DistType::ChiSquare): {
            chi_tint(); 
        } case (DistType::Nonrandom): {
            alpha_zero_tinting();
          }
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
