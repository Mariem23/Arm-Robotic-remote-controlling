#include <Servo.h>
void setup(){
  
  
  
  
  
  }

void loop() {
void init_servos() {                // initialisation des servos sur le consigne de depart

  byte i;
  for (i = 0; i < nb_servos; i++)
  {
    output_bit(tabpin[i],PULSE_OFF);
    tab_pos[i]=tabcons[i];
    tab[i]= servocode(tabcons[i],mult[i],plus[i]);      // traduction de la valeur pour le timer
    printf(lcd_putc,"\fInit servo");
    delay_ms(200);
  }
}
void initialisation() {
  cpt=0;
  x=0;
  c=0;
  s_actif = 0;
  lcd_init();                   // Initialisation du Display
  cursor(0);                    // curseur OFF
  delay_ms(5000); 
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
  setup_timer_2(T2_DIV_BY_16,103,10);
  enable_interrupts(INT_TIMER2);
  enable_interrupts(INT_TIMER1);
  enable_interrupts(int_rda);
  enable_interrupts(GLOBAL);
  set_Timer1(65535 - tab[0]);         // Initialisation du timer avec la consigne de depart

}
}
