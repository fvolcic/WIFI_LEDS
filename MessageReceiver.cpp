//This is the class that will hold the data for new mqtt messages.

#ifndef receiverCPP
#define receiverCPP
#include "MessageReceiver.h"
#include "Arduino.h"

MessageReceiver::MessageReceiver() {
  message = "";
  newMessage = false;
}

void MessageReceiver::feedMessage(String message) {
  this->message = message;
  newMessage = true;
}

bool MessageReceiver::getNewMessage() {
  return newMessage;
};

String MessageReceiver::getMessage() {
  newMessage = false;
  return message;
}


#endif
