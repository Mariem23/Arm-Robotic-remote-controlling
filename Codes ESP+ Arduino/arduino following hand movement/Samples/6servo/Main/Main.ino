#define ESPSTATE
#include "Libraries.h"

// Bonswar Vous, bienvenue dans mon monde du bonheur


char buffer[10] = {0};  // This is needed only if you are going to receive a text from the display. 


void setup() {
      //Port Serial
      Serial.begin(9600);
  

      //Servo motor
      SetupServoArm(); 
      clock_begin = millis();
}

void loop() {
   clock_end = millis();   
 
    if (clock_end > clock_begin + 1,77) {
    clock_begin = clock_end;
    
     MoveMyServo(1);
//     MoveMyServo(2);
//     MoveMyServo(3);
//     MoveMyServo(4);
//     MoveMyServo(5);
//     MoveMyServo(6); 
     
    }
      
     
     Serial.print(SPEEDS[1]);
     Serial.print (':');
     Serial.print(TARGET[1]);
     Serial.print (':');
     Serial.println(REAL[1]);
     
}

