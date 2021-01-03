//This is the executer for the LED actions. This runs each led action, and manages everything for the LEDs.
//All the actions are stored by the executer/

#ifndef ActionExecuter_Gaurd
#define ActionExecuter_Gaurd
#include "LedAction.h"
#include <APA102.h> //The led library
#include <MQTT.h>

class ActionExecuter {

  private:

    //This will point to an array of actions that are to be executed.
    //Will be managed dynamically
    LED_Action **actionList;

    //The total number of actions that are currently contained in the this->actionList
    uint8_t numActions;

    //This is the array that will store what the LED strip colors are before they are written to the strip
    rgb_color * colors;

    //A reference to the APA102 LED Strip for IO output
    APA102<13, 12> &ledStrip;

    //The number of LEDs in the strip
    const uint16_t num_leds;

    //determines if the LEDs need to be updated
    bool updateLEDs;

    //deteremine if the LEDs have alternate core actions
    bool alternateActions;

    //determine if the LEDs have alternate core MQTT actions
    bool alternateMQTTActions;

    //The brightness that the led strip will use
    int brightness;


  public:

    //Default constructor
    ActionExecuter();

    //The default constructor for the action executer
    ActionExecuter(APA102<13, 12> &ledStrip, const uint16_t num_leds);

    //Add an execution action
    //REQUIRES: LED_Action is a valid action
    //EFFECTS: an LED_Action will be added to the end of the action list array.
    //         the current this->actionList array will be deleted, and a new one will be created with the new action
    void add_LED_action(LED_Action * action);

    //Remove an action from the LED_action list.
    //REQUIRES: LED_Action_index points to a valid index in the array.
    //          this->actionList must contain at least one action in it
    //EFFECTS: Removes an action from the LED Action array
    void remove_LED_action(int LED_Action_index);

    //Clears the entire LED action list
    //REQUIRES: nothing is required
    //EFFECTS:The this->actionList array will be cleared. All actions contained within the list are deleted.
    //       :The list itself is then also deleted
    void clear_LED_actions();


    //changes the LED Action at a certain index in the LED Action list.
    //REQUIRES: LED_Action_index points to a valid index in the list
    //        : LED_Action is a valid action
    //EFFECTS: The current LED_Action at LED_Action_index is changed to action
    void edit_LED_action(int LED_Action_index, LED_Action * action);

    //This sequentially executes all actions in the led action list, writing to the IO ports.
    //This is the only function that will write directly to the LEDs
    void executeActions();

    //This is for the alternate action executer that runs on the internet core
    void executeAlternateActions();

    //Returns the current number of LEDs
    uint16_t getNumLEDs();

    //set the number of LEDs
    void setNumLEDs(uint16_t led_count);

    //this returns the colors array that is used when changing the LED colors.
    rgb_color * getColors();

    void setBrightness(int brightness);

    //The deconstructor
    ~ActionExecuter();

};


#endif
