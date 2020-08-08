//This is the class that will hold the data for new mqtt messages.

#ifndef receiverH
#define receiverH
#include "Arduino.h"

class MessageReceiver {
  private:
    bool newMessage;
    String message;

  public:

    MessageReceiver();

    void feedMessage(String message);

    bool getNewMessage();

    String getMessage();
};


#endif
