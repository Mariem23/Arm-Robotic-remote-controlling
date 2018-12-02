#include "Libraries.h"

void setup() {
      //Servo
      SetupServoArm(); 
      clock_begin = millis();
      //configuration Wifi 
      UDPSetup();
}

void loop() {
  //Mouvement servo
  clock_end = millis();   
 
  if (clock_end > clock_begin + 1,77) {
    clock_begin = clock_end;

    UDP_Reception();
    MoveMyArm();
  }
     
}

