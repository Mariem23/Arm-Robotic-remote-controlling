// program to test Servomoteur
// Mariem Fattoum
#include <Servo.h>
Servo Monservo; /**déclaration du servomoteur**/
 int valeur; /**valeur sur arduino**/  
 int potentiometre= A0; /**potentiomètre loué à la pate mise sur la carte**/
 
void setup() {
 Monservo.attach (8); /**attachement du servo à la pin qui lui correspond**/ 
}

void loop() {
  valeur = analogRead (potentiometre); /** lecture de la valeur d'entrée du potentiometre**/ 
  valeur= map(valeur,0,1023,0,159); /** variation de l'angle du servomoteur**/
  Monservo.write (valeur); /** en écriture on demande au servo de prendre la valeur qu'on lui a associé**/  
  delay(15); /**suspendre le programme pour une durée de 15 millisecondes **/ 
}
