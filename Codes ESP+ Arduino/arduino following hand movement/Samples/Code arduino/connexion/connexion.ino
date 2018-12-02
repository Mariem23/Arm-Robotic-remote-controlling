int16 const tabcons[6] ={120,120,90,50,80,100}; // définition des consignes
volatile byte tabpin[6]={pin_d7,pin_d6,pin_d5,pin_d4,pin_d3,pin_d2}; // définition des PIN's
#include <Servo.h>
Servo myservo;  // définition de l'objet myservo

void setup() {
  // put your setup code here, to run once:

}

void loop() {
 void init_servos() {               // initialisation des servos sur le consigne de depart
  byte i;
  for (i = 0; i < nb_servos; i++)
  {
    output_bit(tabpin[i],PULSE_OFF);
    tab_pos[i]=tabcons[i];
    tab[i]= servocode(tabcons[i],mult[i],plus[i]);      // traduction de la valeur pour le timer
    printf(lcd_putc,"\fInit servo");
    delay_ms(200);
  }
  void step_servo(byte serv,byte out,byte in){        // commande de configurattion d'un servo
    
  if (out < in)                       // Vérification de la consigne de sortie: plus grand ou plus petite que la consigne d'entrée (position précédent) ?
  {
    tab_pos[serv]=tab_pos[serv]-step;           // Décrémentation de la position selon un certain pas "step"
    tab[serv]=servocode(in,mult[serv],plus[serv]);
    
    
  }
  
  if (out > in) 
  {
    tab_pos[serv]=tab_pos[serv]+step;
    tab[serv]=servocode(in,mult[serv],plus[serv]);  
  }

  if(out==in)                       // Si entrée = sortie => On met un "1" dans la case du tableau correspondant au servomoteur pour indiqué que celui-ci est arrivé à sa postiion finale
  {
    check_out[serv]=1;
  }
  else
  {
    check_out[serv]=0;
  }

}







}
