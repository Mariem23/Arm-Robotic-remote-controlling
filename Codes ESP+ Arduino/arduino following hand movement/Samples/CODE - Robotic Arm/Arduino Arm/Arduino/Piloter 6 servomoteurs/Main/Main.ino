//#define ESPSTATE
#include "Libraries.h"
void setup() {
      //Port Serial;
      Serial.begin(9600);
      Servo motor; 
      SetupServoArm(); 
      clock_begin = millis();
}

void loop() {
   clock_end = millis();   //vitesse
 
    if (clock_end > clock_begin + 1,77) { 
    clock_begin = clock_end;
    
     MoveMyServo(1);
     MoveMyServo(2);
     MoveMyServo(3);
     MoveMyServo(4);
     MoveMyServo(5);
     MoveMyServo(6); 
     
    }
      
     
     Serial.print(SPEEDS[1]);
     Serial.print (':');
     Serial.print(TARGET[1]);
     Serial.print (':');
     Serial.println(REAL[1]);
     
}

