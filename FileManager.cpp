#ifndef FileManagerCppGaurd
#define FileManagerCppGaurd
#include "FileManager.h"
#include "Arduino.h"
#include <String.h>
FileManager::FileManager(): led_count(240), deviceName(""), defaultActionKey(""), defaultActionMessage(""), defaultAction(false) {
  //ATTh2rZ2VS
  ssid[0] = 'A'; ssid[1] = 'T'; ssid[2] = 'T'; ssid[3] = 'h'; ssid[4] = '2'; ssid[5] = 'r'; ssid[6] = 'Z'; ssid[7] = '2';  ssid[8] = 'V';  ssid[9] = 'S';
  //#?aswkvqhdjq
  pass[0] = '#'; pass[1] = '?'; pass[2] = 'a'; pass[3] = 's'; pass[4] = 'w'; pass[5] = 'k'; pass[6] = 'v'; pass[7] = 'q'; pass[8] = 'h'; pass[9] = 'd'; pass[10] = 'j'; pass[11] = 'q';


}


void FileManager::loadData() {}


void FileManager::storeData(char * entryName, char * datum) {}

#endif
