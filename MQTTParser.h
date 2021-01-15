#ifndef MQTTPARSER_H
#define MQTTPARSER_H

//This is the class for the MQTT Parser and MQTT Mangager
#include <MQTT.h>
#include <ArduinoJson.h>
#include "ActionExecuter.h"
#include "LedAction.h"
#include "SolidColorAction.cpp"
#include "Gradient.cpp"
#include "GradientMoving.cpp"
#include "cstrcmp.h"
#include "LEDPoint.cpp"
#include "SolidColorTransition.cpp"
#include "uninitilizedData.cpp"
#include "OTA_Updater.h"
#include "BrightnessGradient.cpp"
//#include "SubAction.cpp"

extern void update_firmware();

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


    //If there is a new brightness available
    bool brightnessAvailable;
    int brightness;

    //If the action manager should send out an mqtt message to a specified topic
    bool sendActionsToMqtt = false; 
    char * topic = new char[0]; 

    //This is for reading JSON style messages. Library used is ArduinoJson, which can be found at arduinojson.org
    //This sets the json document parser on the stack with a fixed amount of memory. In this case, it is 500 bytes.
    StaticJsonDocument<500> doc;

  public:

    //The constructor
    MQTTParser(int led_count); 



    //MESSAGE FORMAT
    // 3 BYTES - LENGTH OF MESSAGE
    // 4 BYTES - ACTION KEY
    // 116 BYTES - MESSAGE INFORMATION
    // 2 BYTES - MESSAGE END (##)
    // EXAMPLE MESSAGE - 6E00010001009SOLI*********##

    void set_led_count(int led_count);
    
    //This is what parses the MQTT message.
    //This is for parsing default style messages for this device. These messages are ultralight weight
    void parseMessageNormal(String &message);

    //This formulates an action after parse message has been called.
    //REQUIRES: LED Count
    //EFFECTS: creates an action that the action pointer will point to.
    void createAction(const uint8_t led_count);


    //This function determines what style the message is,
    //and from that then passes the message to the proper
    //message parser.
    void parseMessage(String message); 
    
    //this is for parsing json style messages

    //This works by using the information that is in the JSON style message and returning it as something
    //that can be read using the normal protocol.
    //The best way to determine the type of message that has been received (JSON or normal),
    //it is easiest to just check if the received message starts with '{'
    //if it does not contain that character, then attempt to parse message normally.
    void parseJSONMessage(String message);


    //This is run once the message has been parsed.
    //This will add actions if there are available actions.
    //This function takes in the manager pointer, and
    //With that manages the led actions.
    void manageLEDs(ActionExecuter * manager);


    //Checks if there is a new brightness
    bool getBrightnessAvailable();

    //Return the brightness
    int getBrightness();

    //This returns the action after an action was created
    //REQUIRES: create action has already been called, and there is an available action waiting
    //EFFECTS: Returns the current action that is being pointed to by the action pointer.
    LED_Action * getCreatedAction();

    //getter function for if there is an available action.
    bool getActionAvailable();

    //getter for if there is an action that is to be edited
    bool getEditAction(); 

    //Returns the index that is to be edited
    int getEditIndex();
    
    //getter for if there was a command to clear all current running device actions
    bool getClearActions();

    bool getKillAction();

    int killActionIndex();

    bool getSendMQTTActionList(); 
};


#endif
