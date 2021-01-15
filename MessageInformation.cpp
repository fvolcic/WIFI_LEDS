#include "MessageInformation.h"
#include <stdlib.h>
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

#define C_LENGTH 9
#define N_LENGTH 1
#define COlON_LENGTH 1

#define C_ELT_LENGTH 1
#define N_ELT_LENGTH 1
#define COlON_ELT_LENGTH 2

inline int getEltLength(char type);
inline int getLength(char, char *, int);

//The constructor
MessageInformation::MessageInformation(int numberOfMessageElts, char *messageElts, char *message) : numberOfMessageElts(numberOfMessageElts), messageElts(messageElts), message(message)
{
}

//O(n) random access
//O(1) sequential access
char *MessageInformation::getElt(int index)
{

    if (index == this->eltIndex)
        return getSequentialAccessElt(index);

    int iterator = 0;
    int elt_index = 0;
    for (int i = 0; i < index; ++i)
    {
        iterator += getLength(messageElts[elt_index], messageElts, elt);
        elt_index += getEltLength(messageElts[elt_index]);
    }

    return message + iterator;
}

char *MessageInformation::getSequentialAccessElt(int index)
{
    int temp = msgIndex;
    
    ++eltIndex; 
    msgIndex += getLength(messageElts[elt], messageElts, elt);
    elt += getEltLength(messageElts[elt]);

    return message + temp; 
}

//Returns the number of characters that a particular
//Character takes up
inline int getLength(char type, char * eltMsg, int eltIndex)
{

    switch (type)
    {
    case 'C':
        return C_LENGTH;

    case 'N':
        return N_LENGTH;

    case ':':
        return (int)(eltMsg[eltIndex + 1] - '0');
    }

    return 0;
}

inline int getEltLength(char type)
{

    switch (type)
    {
    case 'C':
        return C_ELT_LENGTH;

    case 'N':
        return N_ELT_LENGTH;

    case ':':
        return COlON_ELT_LENGTH;
    }
}


int MessageInformation::getColorValue(char col, char * colMsg){

    char colDefiner[3] = {0,0,0};

    switch (col)
    {
    case 'G':
        colMsg += 3; 
        break;
    
    case 'B':
    colMsg += 6; 
        break;
    }

    colDefiner[0] = colMsg[0]; 
    colDefiner[1] = colMsg[1]; 
    colDefiner[2] = colMsg[2]; 

    return atoi(colDefiner); 
}


 int getValue(char * startingIndex, int numLength){

  char * tmpNum = new char[numLength];

  for(int i = 0; i < numLength; ++i){
    tmpNum[i] = startingIndex[i]; 
  }

  int returnVal = atoi(tmpNum); 

  delete tmpNum; 

  return returnVal; 
  
 }

/*
#include <iostream>
#include <string> 

using namespace std;

int main(){

    char seq[] = ":2C:3N";
    char message[] = "12ABCDEFGHI5553";

    MessageInformation msg(4, seq, message); 

    for(int i = 0; i < 4; ++i){

        int msgLength = getLength(seq[msg.elt], seq, msg.elt); 
        const char * begin = msg.getElt(i); 

        for(int j = 0; j <msgLength;++j){

            cout << begin[j]; 

        }

        cout << endl; 
    }



}
*/
