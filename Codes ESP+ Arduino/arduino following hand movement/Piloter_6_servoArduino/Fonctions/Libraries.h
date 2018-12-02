#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <Servo.h>

//Define type of servo

#define NSERVO 7
extern Servo SERVOS[NSERVO];

#ifdef ARDUINO
const int PINS[NSERVO] = {2,4,7,8,12,13};
const int INIT[NSERVO] ={0,900,1370,900,1600,900,1200};
const int MAX[NSERVO] ={0,2000,1370,1750,800,2000,2000};
#else  //ESPSTATE
const int PINS[NSERVO] = {0,5,4,0,2,14,12};
const int INIT[NSERVO] = {0,0,0,20,100,40,70};
const int MAX[NSERVO] = {0,180,180,140,0,120,180};
#endif
 
extern int REAL[NSERVO];  
extern int TARGET[NSERVO];  
extern byte SPEEDS[NSERVO];


//Definition des variables

extern unsigned long clock_begin;
extern unsigned long clock_end;


//Definition de la fonction
void MoveMyServo(int num);

#endif



