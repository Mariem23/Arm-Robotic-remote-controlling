// program to test Servomotor
// date 22/05/2018
// Mariem Fattoum

#include <Servo.h>


int rotationServo(int numServ, int start, int end) {
   for (; start<end; start++) {
      longueurImpulsion = map(start, 0, 180, SERVOMIN, SERVOMAX );
      pwm.setPWM(numServ, 0, longueurImpulsion); 
      delay(15);
  }
}

void setup() 
{}
void loop() { 
  if ( angleArrive-angleDepart > 0){ 
    angleDepart = rotationServo(numServ, angleDepart, angleArrive);
  } 
  else {
    angleArrive = rotationServo(numServ, angleArrive, angleDepart);
  }
}

