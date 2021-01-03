#ifndef actionExecuterCode
#define actionExecuterCode
#include "ActionExecuter.h"

//-------------------------------------------------------------------------------------------------------------------------
//
//                                                METHODS ARE DEFINED BELOW
//
//-------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------|constructor|-----------------------

//The default constructor for the action executer
ActionExecuter::ActionExecuter(APA102<13, 12> &ledStrip, uint16_t num_leds) : ledStrip(ledStrip), num_leds(num_leds), brightness(8) {
  this->actionList = nullptr;
  this->numActions = 0;
  this->colors = new rgb_color[num_leds];
  this->updateLEDs = false;
  this->alternateActions = false;
  this->alternateMQTTActions = false;

}

//------------------------------------------------------------------------------------------------------------------|add_LED_action|-----------------------

//Add an execution action
//REQUIRES: LED_Action is a valid action
//EFFECTS: an LED_Action will be added to the end of the action list array.
//         the current this->actionList array will be deleted, and a new one will be created with the new action
void ActionExecuter::add_LED_action(LED_Action * action) {

  Serial.println("SETTING UP ACTION");

  //Determine if this action will overwrite all past LED actions.
  //If so, we can safely clear all the old led actions.
  if (action -> getFullStripUpdate())
    this->clear_LED_actions();


  //Run all the setup functions for the current action that is being added to the list
  action->setupLEDs(this->colors);

  action->alternateCoreActionSetup();


  //This determines if any actions need to be run
  if ((action -> getUpdateLEDs()))
    updateLEDs = true;

  if (action -> getAlternateCoreAction())
    alternateActions = true;

  if (action -> getMQTTAction())
    alternateMQTTActions = true;

  //If the array is empty, just add the action to the action list
  if (this->numActions == 0) {
    this->numActions ++;
    this->actionList = new LED_Action * [this->numActions];
    this->actionList[0] = action;
    return;
  }


  //temporarily store the action list
  LED_Action ** tmpactionList = this->actionList;

  ++ this->numActions;

  //Instantiate new list of actions
  this->actionList = new LED_Action * [this->numActions];

  //Copy over all the old actions into the new array
  for (int i = 0; i < this->numActions - 1; i++) {
    this->actionList[i] = tmpactionList[i];
  }

  this->actionList[this->numActions - 1] = action;

  delete [] tmpactionList;
}

//------------------------------------------------------------------------------------------------------------------|remove_LED_action|-----------------------

//Remove an action from the LED_action list.
//REQUIRES: LED_Action_index points to a valid index in the array.
//          this->actionList must contain at least one action in it
//EFFECTS: Removes an action from the LED Action array
void ActionExecuter::remove_LED_action(int LED_Action_index) {

  //If the action does not exsist, just return from the function altogether
  if (LED_Action_index >= this->numActions)
    return;

  //Create a temporary array to store current action data
  LED_Action ** tmpactionList = this->actionList;

  //Allocate new this->actionList
  this->actionList = new LED_Action * [this->numActions - 1];

  //This temporarily stores the action that is to be removed.
  //This will allow a deletion of the action later
  LED_Action * tmpAction = tmpactionList[LED_Action_index];

  //Run the deconstructors for the action
  tmpAction -> deconstruct_displayLEDs();
  tmpAction -> deconstruct_alternateCoreAction();
  //  tmpAction -> deconstruct_alternateCoreMQTTAction(client);


  int offset = 0;

  //Copy over old actions, excluding the action that is to be removed.
  for (int i = 0; i < this->numActions; ++i) {
    if (i == LED_Action_index && i != this->numActions - 1) {
      ++i;
      offset = -1;
    }
    else if (i == this->numActions - 1 && i == LED_Action_index)
      break;

    this->actionList[i + offset] = tmpactionList[i];
  }

  numActions --;

  //Remove the no longer needed values
  delete [] tmpactionList;
  delete tmpAction;


  //Check if any actions need to be run after an action has been deleted
  updateLEDs = false;
  alternateActions = false;
  alternateMQTTActions = false;

  for (int i = 0; i < numActions; ++i) {
    if (actionList[i] -> getUpdateLEDs())
      updateLEDs = true;

    if (actionList[i] -> getAlternateCoreAction())
      alternateActions = true;

    if (actionList[i] -> getMQTTAction())
      alternateMQTTActions = true;

  }


}



//------------------------------------------------------------------------------------------------------------------|clear_LED_actions|-----------------------
//Clears the entire LED action list
//REQUIRES: nothing is required
//EFFECTS:The this->actionList array will be cleared. All actions contained within the list are deleted.
//       :The list itself is then also deleted
void ActionExecuter::clear_LED_actions() {
  for (int i = 0; i < numActions; i ++) {

    //Run the deconstructor for each action type
    this->actionList[i]->deconstruct_displayLEDs();
    this->actionList[i]->deconstruct_alternateCoreAction();
    //this->actionList[i]->deconstruct_alternateCoreMQTTAction(client);

    //Delete the action
    delete this->actionList[i];
  }

  delete [] this->actionList;
  this->actionList = nullptr;
  this->numActions = 0;
}


//------------------------------------------------------------------------------------------------------------------|edit_LED_action|-----------------------
//changes the LED Action at a certain index in the LED Action list.
//REQUIRES: LED_Action_index points to a valid index in the list
//        : LED_Action is a valid action
//EFFECTS: The current LED_Action at LED_Action_index is changed to action
void ActionExecuter::edit_LED_action(int LED_Action_index, LED_Action * action) {
  if (LED_Action_index >= numActions)
    return;

  //Determine if the edited action calls for removing all the past actions.
  if (action -> getFullStripUpdate()) {
    for (int i = 0; i < LED_Action_index; i ++) {
      this->remove_LED_action(0); //remove the first action each time
    }

    //Make sure to account for the change in index position.
    LED_Action_index = 0;
  }


  //Run the deconstructor for each action type
  this->actionList[LED_Action_index]->deconstruct_displayLEDs();
  this->actionList[LED_Action_index]->deconstruct_alternateCoreAction();
  //    this->actionList[LED_Action_index]->deconstruct_alternateCoreMQTTAction(client);

  delete actionList[LED_Action_index];

  actionList[LED_Action_index] = action;

  //Run all the setup functions for the current action that is being added to the list
  action->setupLEDs(this->colors);
  action->alternateCoreActionSetup();
  //       action->alternateCoreMQTTActionSetup(client);

  //Check if any actions need to be run after an action has been deleted
  updateLEDs = false;
  alternateActions = false;
  alternateMQTTActions = false;

  for (int i = 0; i < numActions; ++i) {
    if (actionList[i] -> getUpdateLEDs())
      updateLEDs = true;

    if (actionList[i] -> getAlternateCoreAction())
      alternateActions = true;

    if (actionList[i] -> getMQTTAction())
      alternateMQTTActions = true;

  }
}


//------------------------------------------------------------------------------------------------------------------|executeActions|-----------------------

//This sequentially executes all actions in the led action list, writing to the IO ports.
//This is the only function that will write directly to the LEDs
void ActionExecuter::executeActions() {
  //run each action again, but only if there is an action that requires this
  if (updateLEDs) {
    for (int i = 0; i < numActions; i ++) {
      actionList[i]->displayLEDs(colors);
    }
  }

  bool writePrepared = true; //This is to ensure that every action is ready to be written to the led strip

  //Run through each action and make sure the action is prepared
  for (int i = 0; i < numActions; ++i) {
    if (!actionList[i]->getWritePrepared()) {
      writePrepared = false;
      break;
    }
  }

  //This writes all the colors to the led strip
  //there is an optional third paramater, which is brightness, which could be used.
  //Instead of using worrying about each LED brightness, the brightness is just default set to max
  //This is useful for lowering the amount of used memory, along with how quickly everything can run
  //Any brightness can still be acheived, it just must be scaled in the RGB color values.
  //(ex. color(255, 255,255) brightness = 15 is approx. equal to color(128,128,128) brightness = 30

  //Serial.print("WRITING TO LED STRIP NOW: led_count: ");
  //Serial.println(num_leds);

  if (writePrepared)
    this->ledStrip.write(colors, num_leds, brightness);

}

//------------------------------------------------------------------------------------------------------------------|executeAlternateActions|-----------------------

//This is for the alternate action executer that runs on the internet core
void ActionExecuter::executeAlternateActions() {

  //If there are alternate actions that need to be run
  if (alternateActions) {
    for (int i = 0; i < numActions; i ++) {

      //Run the deconstructor for each action type
      this->actionList[i]->deconstruct_displayLEDs();
      this->actionList[i]->deconstruct_alternateCoreAction();
      //this->actionList[i]->deconstruct_alternateCoreMQTTAction(client);

      actionList[i]->alternateCoreAction();
    }
  }

  /*
    //If there are alternate MQTT actions that need to be run
    if(alternateMQTTActions){
    for(int i = 0; i < numActions; i ++){
       actionList[i]->alternateCoreMQTTAction(this->client);
    }
    }
  */
}

//------------------------------------------------------------------------------------------------------------------|getColors|-----------------------

//this returns the colors array that is used when changing the LED colors.
rgb_color * ActionExecuter::getColors() {
  return this->colors;
}

//------------------------------------------------------------------------------------------------------------------|getNumLEDs|-----------------------


uint16_t ActionExecuter::getNumLEDs() {
  return this->num_leds;
}

//------------------------------------------------------------------------------------------------------------------|getNumLEDs|-----------------------

//set the number of LEDs
void ActionExecuter::setNumLEDs(uint16_t led_count) {
  delete [] colors;
  colors = new rgb_color[led_count];
}

//------------------------------------------------------------------------------------------------------------------|Deconstructor|-----------------------

//The deconstructor. Cleans up all dynamic memory.
ActionExecuter::~ActionExecuter() {

  //Delete all current actions
  for (int i = 0; i < numActions; i ++) {

    //Run the deconstructor for each action type
    this->actionList[i]->deconstruct_displayLEDs();
    this->actionList[i]->deconstruct_alternateCoreAction();
    //this->actionList[i]->deconstruct_alternateCoreMQTTAction(client);


    delete actionList[i];
  }

  //delete the dynamic action list
  delete [] actionList;
}


//------------------------------------------------------------------------------------------------------------------|Strip Brightness|-----------------------

void ActionExecuter::setBrightness(int brightness) {
  this->brightness = brightness;
}

#endif
