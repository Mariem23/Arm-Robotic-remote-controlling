#include <Servo.h> // appel à la biblio servo
Servo monservo; //instance de l'objet servo
int bouton=8; // bouton poussoir attaché à la pin8
int etatbouton=0; // une variable qui prend la valeur 0 ou 1 selon l'état du bouton ouvert/fermé

void setup() {
  pinMode(bouton, INPUT); // l'entrée
  monservo.attach(9); // attachement du servo au pin 9

}

void loop() {
  etatbouton=digitalRead(bouton); // lecture de l'etat
  if (etatbouton==HIGH) //on test si l'état bouton + , le servo prend l'angle de 90 degrés
  {
      monservo.write(90);
      delay(5000); // attente de 5 secondes
      monservo.write(0); // reprendre l'état initiale
      delay(150); // attente de 150 millisecondes puis rebelote!!!! 
    
    }
}
