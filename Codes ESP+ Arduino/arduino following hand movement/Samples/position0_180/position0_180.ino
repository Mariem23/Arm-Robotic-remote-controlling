#include <Servo.h>

int pinServo=8; // variable pour stocker le pin pour la commande
Servo monservo; // on définit un objet Servo 
void setup() {
  monservo.attach(pinServo); // on relie l'objet au pin de commande
}

void loop() {
  monservo.write(0); // on dit à l'objet de mettre le servo à 0°
  delay(1000); // ce délai est nécessaire pour que le servo atteigne sa position
  monservo.write(179); // position à 179, 180 est à éviter car cela forcerait le servo à dépasser ses limites
  delay(1000); // attente à nouveau
}
