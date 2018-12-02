#include "Libraries.h"


//Initialisation des valeurs 
int  val = 0;
int tmp = 0;
float threshold = 0.4;
unsigned long clock_begin=0;
unsigned long clock_end=0;
unsigned long clock_begin2 = 0;
unsigned long clock_end2 = 0;
unsigned long clock_delta = 0;


//nom et type des servos
Servo SERVOS[NSERVO];
int REAL[NSERVO];
int TARGET[NSERVO];
byte SPEEDS[NSERVO];



void servoSetup(int num) {

  SERVOS[num].attach(PINS[num]); // attachement au Pin's
  TARGET[num] = INIT[num];
  REAL[num] = INIT[num];
  SPEEDS[num] = 1;
  SERVOS[num].writeMicroseconds(INIT[num]);
}

void SetupServoArm() {

  for (int num = 1; num < 7; num++) {
    servoSetup(num);
  }
}
void MoveMyServo(int num) {

    if (SPEEDS[num] = 13) {
      TARGET[num]=REAL[num];
    }
    else if (SPEEDS[num] > 0) {
     if (REAL[num] > TARGET[num]) REAL[num] = ( REAL[num] + TARGET[num] )/2;
     REAL[num] = REAL[num] + SPEEDS[num];
  }
    
    else {
      REAL[num] = REAL[num] - 1;
      if (REAL[num] < INIT[num]) REAL[num] = INIT[num];
    }
 //SERVOS[num].writeMicroseconds(REAL[num]? REAL[num] : MAX[num]-REAL[num]);
}


