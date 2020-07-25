//This is for creating actions that the leds can use
//This is an abstract class, and every action should inheart this class

#ifndef LED_Action_GAURD
#define LED_Action_GAURD
#include <APA102.h> //The led library
#include <MQTT.h>

class LED_Action{
  protected:
  
    int led_count; //The number of LEDs that exist in the LED strip 
    bool updateLEDs; //This tells the program is the LEDs need to be written to each loop, or if the task can be killed
    bool alternateCoreActions; //This lets the program know if there are actions that need to be run on an alternate core
    bool MQTTAction; //Lets the program know if there are MQTT Actions that need to be run
    char * message; 
    bool preparedForWrite; //This is to let the action executer know if the action has finished running commands, and if it is now ready for update
    char * MQTTTopic; //This is for if there is a MQTT Topic that an action needs to use to run
    
  public: 

    //Default constructor
    LED_Action(int led_count,  bool updateLEDs, bool alternateCoreActions, bool MQTTAction,char * message) : 
    led_count(led_count),  updateLEDs(updateLEDs), alternateCoreActions(alternateCoreActions), MQTTAction(MQTTAction),message(message),preparedForWrite(false) {}
    
    //This is the function that is called to both update an array containing the LED colors
    //REQUIRES: rgb_color points to the start of an rgb_color array, with length equal to the private variable led_count
    //EFFECTS: the colors array will be updated and the newest rgb_color values will be added to the array
    virtual void displayLEDs(rgb_color * colors) = 0;

    //The setup function for an action. This is called a single time when the action is instantiated. 
    //REQUIRES: a valid message
    //EFFECTS: Runs the setup code for the alternate core actions 
    virtual void setupLEDs(rgb_color * colors) = 0; 

    //The setup function for the alternate core.
    //REQUIRES: a valid message
    //EFFECTS: Runs the setup code for the alternate core actions 
    virtual void alternateCoreActionSetup() = 0; 

    //This is the function that is called to run on the alternate core. Alternate core is referring to the core that is managing mqtt communications,
    //and not LED lights. Things that should be run here are tasks that may lag due to the internet. This will ensure that the LEDs never
    //lose their smoothness then running. 
    //If there is no action needed for the alternate core, then leave this action blank. 
    virtual void alternateCoreAction() = 0; 

    //The setup function that runs on the internet core. This is used when an there is a MQTTClient object that 
    //should be used instead of creating a new client
    //REQUIRES: A valid MQTT Client and a valid MQTT message
    //EFFECTS: runs the setup function for the alternateCoreMQTTActionSetup
    //virtual void alternateCoreMQTTActionSetup(MQTTClient &client) = 0;

    //This is the function that is called when the internet core action manager calls this action from the action cue
    //virtual void alternateCoreMQTTAction(MQTTClient &client) = 0; 

    //A deconstruction function for normal LED actions
    virtual void deconstruct_displayLEDs() = 0;

     //A deconstruction function for the alternate core actions
    virtual void deconstruct_alternateCoreAction() = 0;

     //A deconstruction function for the MQTT Actions 
     //virtual void deconstruct_alternateCoreMQTTAction(MQTTClient &client) = 0;
    

    
    virtual ~LED_Action(){};
    //getter function for updateLEDs
    bool getUpdateLEDs(){
      return updateLEDs; 
    }

    //getter function for the alternateCoreActions
    bool getAlternateCoreAction(){
      return alternateCoreActions; 
    }

    //getter function for MQTTAction
    bool getMQTTAction(){
      return MQTTAction; 
    }
    
    //Getter function for returning the number of LED strips in the LED
    int getLEDCount(){
      return led_count; 
    }
    
    bool getWritePrepared(){
      if(preparedForWrite){

        if(updateLEDs)
          preparedForWrite = false;
        return true;
      }
        
      return false;
    }
    
    
};
#endif
