#ifndef cstrcmpCPP
#define cstrcmpCPP

//#include <SoftwareSerial.h>
bool cstrcmp(char * c_str1, char * c_str2) {


  //Serial.println("Runnning cstrcmp while loop");
  while (*c_str1 && * c_str2) {
    if (*c_str1 != *c_str2)
      return false;

    //Serial.println("Increment");
    ++c_str1;
    ++c_str2;
  }

  // Serial.println("Left while loop");
  if ( (!*c_str1 && *c_str2) || (*c_str1 && !*c_str2))
    return false;



  return true;
}


#endif
