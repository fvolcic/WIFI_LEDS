#ifndef FileManagerHeaderGaurd
#define FileManagerHeaderGaurd
#include "Arduino.h"
class FileManager {
  private:

    uint16_t led_count; //The number of leds on the strip
    char deviceName[64]; //The default name of the device
    char defaultActionKey[5]; //A string containing the default action name
    char defaultActionMessage[129]; //What the default action startup message is
    bool defaultAction; //Is there a default action or not
    char ssid[32];
    char pass[32];



  public:

    //The constrictor for the file manager
    FileManager();

    //Load all the data from the storage locations in SPIFFS
    void loadData();

    //Store a peice of data in SPIFFS
    void storeData(char * entry, char * datum);

    bool getDefaultAction() {
      return defaultAction;
    }

    char * getDefaultActionKey() {
      return defaultActionKey;
    }

    char * getDefaultActionMessage() {
      return defaultActionMessage;
    }

    char * getDeviceName() {
      return deviceName;
    }

    char * getSSID() {
      return ssid;
    }

    char * getPSK() {
      return pass;
    }

    uint16_t get_led_count() {
      return led_count;
    }


};

#endif
