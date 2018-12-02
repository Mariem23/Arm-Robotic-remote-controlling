
#include <Servo.h>

Servo myservo;  // création d'un objet servo
int pos = 0;    // Variable pour stocker la position du servo

void setup() {
  myservo.attach(9);  // attachement du servo au pin
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { 
    // in steps of 1 degree
    myservo.write(pos);              // aller à la posoition "pos"
    delay(15);                       //attendre 15ms pour que le servo arrive à la position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // un travail à l'envers pour retourner à la position initiale
    myservo.write(pos);              
    delay(15);                       
  }
}
