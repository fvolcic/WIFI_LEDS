/* 
 *  
 *  This code will deconstruct specific messages that are sent to the LEDS 
 *  This will inprove the speed in writing new LED actions as much of the difficulty actually comes from dectronstructing these values
 *
 * IMPORTANT !!!!
 * This class can only be used while the message pointer exists
 * This class must be destroyed at the same time since
 * this class does not generate any of its own dynamic mem
 * !!!
 * 
 */

#ifndef MESSAGEINFORMATIONGAURD
#define MESSAGEINFORMATIONGAURD

 class MessageInformation{

  public: 


    /*
     * messageElts format
     * 
     *  messageElts is a char array of characters that represents the different elements that will be in the message string
     *  
     *  C - a C char represents a 3 character color. A C in the messageElts will correspond to 3 characters in the message array
     *  N - a single number. This will consider only one position in the message array and will be represented as a number
     *  :(number) - the : character is always followed by a number 1 - 255. This represents a number that needs an arbietrary number of characters. For example, :5 represents a 5 char number
     *  
     *  
     */
    MessageInformation(int numberOfMessageElts, char * messageElts, char * message); 

    //This returns a pointer to the starting index of a element.
    //This does all the calculation to reach the index. 
    //REQUIRES: a valid information index that can be accessed
    //EFFECTS: does not change anything
    //RETURNS: a pointer 
    char * getElt(int index); 
  

    //col - either 'R', 'G', or 'B'
    int getColorValue(char col, char * colMsg); 

  private:
    

    char * getSequentialAccessElt(int index); 

    //These are message variables
    //aiding in accessing the message
    int numberOfMessageElts; 
    char * messageElts; 
    char * message;

    //These are used for saving the past state of the getElt
    int elt = 0;
    int msgIndex = 0; 
    int eltIndex = 0; 
    
 }; 

 extern int getValue(char * startingIndex, int numLength); 

 #endif
