// program to test LED
// date 22/05/2018
// Mariem Fattoum
#include <Servo.h>
const int limit;
int val=T0;
Servo monservo;
void setup() {
T0=millis();
}

void loop() {
 T1=millis();
 if (T1-T0)> limit
 digitalWrite(monservo, LOW); 
 T0=T1;
 digitalWrite(monservo,High); 
}
