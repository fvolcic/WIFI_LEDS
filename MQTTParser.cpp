//This is the class for the MQTT Parser and MQTT Mangager
#ifndef MQTTParserGaurd
#define MQTTParserGaurd
#include <MQTT.h>
#include "LedAction.h"
#include "SolidColorAction.cpp"
#include "Gradient.cpp"
#include "GradientMoving.cpp"
#include "cstrcmp.h" 
#include "LEDPoint.cpp"

//Let the compiler know we will be using the cstrcmp function from cstrcmp.h
//extern bool cstrcmp(char * c_str1, char * c_str2);

//MESSAGE FORMAT
// 3 BYTES - LENGTH OF MESSAGE
// 4 BYTES - ACTION KEY
// 118 BYTES - MESSAGE INFORMATION
// EXAMPLE MESSAGE - 6E00010001009


class MQTTParser {
  private:

  //This is the action that the MQTT Parser will create for the action executer
  LED_Action * action;
  
  //If there is a received message, this will parse the message data
  char MQTTMessage[118];

  //The action key of the message
  char actionKey[5]; 

  //Length of the message
  char msgLength[4];
  int int_msgLength;

  //Lets the user know if there is an action waiting to be sent to the action Executer
  bool actionAvailable; 

  //Space to store an entire MQTT Message, along with the '\0' character. 
  char fullMessage[129];

  int led_count; 

  //These are for if there was a call to kill a certain running action
  bool callActionKill; 
  int actionKillIndex; 

  //Check if there was a call to kill all actions
  bool killAllActions; 

  //If there is an action 
  bool callEditAction;
  int editActionIndex; 
  
  public:

  //The constructor
  MQTTParser() : led_count(240), callActionKill(false), actionKillIndex(0), killAllActions(0), callEditAction(false),editActionIndex(0){
    actionAvailable = false;
    MQTTMessage[117] = '\0'; 
    int_msgLength = 0; 
    }


  
//MESSAGE FORMAT
// 3 BYTES - LENGTH OF MESSAGE
// 4 BYTES - ACTION KEY
// 116 BYTES - MESSAGE INFORMATION
// 2 BYTES - MESSAGE END (##)
// EXAMPLE MESSAGE - 6E00010001009SOLI*********##

  void set_led_count(int led_count){
    this->led_count = led_count; 
  }

  //This is what parses the MQTT message.
  void parseMessage(String message){

    callActionKill = false; 
    killAllActions = false; 
    callEditAction = false; 
    
    uint16_t index = 0;
    int messageLength = message.length(); 

    if(message.length() <= 128){
      for(const char * msgPtr = message.c_str(); *msgPtr; ++msgPtr){
         fullMessage[index] = *msgPtr; 
         ++index; 
      }

      fullMessage[index] = '\0';
    }

    char * msgPtr = fullMessage; 

    for(int i = 0; i < messageLength; i++){

      //Store the message length
      if(i >= 0 && i < 3){
        msgLength[i] = *msgPtr; 
      }
      
      if(i == 3){
        msgLength[3] = '\0';// end the first message string
        int_msgLength = atoi(msgLength); 
      }


      if(i >= 3 && i < 7){
        actionKey[i-3] = *msgPtr;
      }

      if(i == 7)
        actionKey[4] = '\0';

      if(i >= 7 && i < 7 + int_msgLength){
        MQTTMessage[i-7] = *msgPtr;
      }

        if(i == (6 + int_msgLength))
            MQTTMessage[int_msgLength] = '\0';

        ++msgPtr;
    }

    
    
  }

  //This formulates an action after parse message has been called. 
  //REQUIRES: LED Count
  //EFFECTS: creates an action that the action pointer will point to. 
  void createAction(const uint8_t led_count){

    Serial.println("Create Action Called");

    //Delete the action if the current stored action was never used. 
    if(actionAvailable)
      delete action; 

    char SolidAction[5] = {'S','O','L','I','\0'};
    char GradientAction[5] = {'G','R','A','D','\0'};
    char MovingGradientAction[5] = {'M','G','R','D','\0'};
    char ClearAction[5] = {'C','L','E','A','\0'};
    char PointAction[5] = {'P','O','N','T','\0'};

    //This detemines what key to use

    Serial.println("Running cstrcmp");
    
    if(cstrcmp(SolidAction, actionKey)){
       action = new SolidColor(led_count,MQTTMessage);
       actionAvailable = true; 
       Serial.println("SOLID COLOR");
    }

    else  if(cstrcmp(GradientAction, actionKey)){
       action = new Gradient(led_count,MQTTMessage);
       actionAvailable = true; 
       Serial.println("GRADIENT");
    }

     else  if(cstrcmp(MovingGradientAction, actionKey)){
       action = new MovingGradient(led_count,MQTTMessage);
       actionAvailable = true; 
       Serial.println("MOVING GRADIENT");
    }
    
    else  if(cstrcmp(PointAction, actionKey)){
       action = new LEDPoint(led_count,MQTTMessage);
       actionAvailable = true; 
       Serial.println("LEDPoint");
    }
    
    else  if(cstrcmp(ClearAction, actionKey)){
      Serial.println("CLEAR");
       killAllActions = true;
    }
    
      else{
         Serial.println("Finished Running cstrcmp");
    actionAvailable = false; 
      }
    
  }


  //This returns the action after an action was created
  //REQUIRES: create action has already been called, and there is an available action waiting
  //EFFECTS: Returns the current action that is being pointed to by the action pointer. 
  LED_Action * getCreatedAction(){
    
    if(actionAvailable){
      Serial.println("Returing Created Action");
      actionAvailable = false; 
      return action; 
    }
    else
      return nullptr; 
     
  }

  //getter function for if there is an available action. 
  bool getActionAvailable(){
    return actionAvailable;
  }

  //getter for if there is an action that is to be edited
  bool getEditAction(){
    return callEditAction;
  }

  //Returns the index that is to be edited
  int getEditIndex(){
    return editActionIndex; 
  }

  //getter for if there was a command to clear all current running device actions
  bool getClearActions(){
    return killAllActions; 
  }

  bool getKillAction(){
    return callActionKill;
  }

  int killActionIndex(){
    return actionKillIndex; 
  }
};


#endif
