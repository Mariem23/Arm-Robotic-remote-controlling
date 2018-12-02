const int angleMin  = 70;
const int angleMax = 110;
 
const byte pinServo = 2; 
const byte pinBouton = 3;

#include <Servo.h> /** inclusion de la bibliothèque **/ 
 

Servo monServo; /** déclaration du servo **/ 
int vitesse;
int angle;
 
void setup()
{
    pinMode(pinBouton, INPUT_PULLUP);
    monServo.attach(pinServo);
    angle = angleMin;
    vitesse = 0;
}
 
/*
 * La fonction loop() est exécutable répétitivement²  
 */
void loop()
{
    if (digitalRead(pinBouton) == LOW && vitesse == 0) {
        vitesse = 1;
    }

    monServo.write(angle);
    /* calcule le nouvel angle */
    angle = angle + vitesse;
 
    if (angle > angleMax) {
        /* 
         * le nouvel angle dÃ©passe le maximum
         * on le recale au maximum et on inverse la vitesse
         */
        angle = angleMax;
        vitesse = -1;
    }
    else if (angle < angleMin) {
        /*
         * le nouvel angle dépasse le minimum
         * on le recale au minimum et on inverse la vitesse
         */
        angle = angleMin;
        vitesse = 0;
    }
    delay(25);
}
