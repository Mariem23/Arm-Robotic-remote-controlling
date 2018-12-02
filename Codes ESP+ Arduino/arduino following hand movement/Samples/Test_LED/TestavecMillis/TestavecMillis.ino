// program to test ......
// date
// Mariem ....
// TODO definir LED BUTTON
//      _VDD HIGH
//      |
//      R
//      |
//      O---PINXXX
//      |
//    |=
//      |
//      -GND LOW

#include <Servo.h>

//#include "MyLED.h"

//--------------------
// MyLED.h
#ifndef MYLED_H
#define MYLED_H
#define LED 7 
#define BUTTON 3
extern boolean LEDstate;
extern void setupLED();
extern void loopLED(boolean blink);
#endif //MYLED_H
//--------------------
// MyLED.ino
long LEDperiod; // variable qui stocke la mesure du LEDperiod
boolean LEDstate;
void setupLED() {
    // on initialise le LEDperiod
    LEDperiod = millis();
    // on attach le moteur au pin
    
    // par défaut la LED sera éteinte
    LEDstate = 0;
    // on éteint la LED
    digitalWrite(LED, LEDstate);
}
void loopLED(boolean blink) {
  static LEDcount = 0;
  if(blink) {    
    // on compare l'ancienne valeur du LEDperiod et la valeur sauvée
    // si la comparaison (l'un moins l'autre) dépasse 1000...
    // ...cela signifie qu'au moins une seconde s'est écoulée
    if((millis() - LEDperiod) > 1000)
    {
        LEDstate = !LEDstate; // on inverse l'état de la LED
        digitalWrite(LED, LEDstate); // on allume ou éteint
        LEDperiod = millis(); // on stocke la nouvelle heure
    }
  }
  else {
      LEDcount++;
       digitalWrite(LED, LOW);
  }
}
//--------------------

void setup(){ 
  setupLED();
}
void loop()
{
  if (LEDstate){
  
  }
    // si le BUTTON est cliqué (on rentre dans un mur)
    if(digitalRead(BUTTON)==HIGH)
    {
        // on arrête le moteur
        loopLED(false); 
    }
    else // sinon on clignote
    {
        // on compare l'ancienne valeur du LEDperiod et la valeur sauvée
        // si la comparaison (l'un moins l'autre) dépasse 1000...
        // ...cela signifie qu'au moins une seconde s'est écoulée
        loopLED(true); 
    }
}
