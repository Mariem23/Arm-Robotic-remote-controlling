//-----------------------------------------------------
//------------ Programme Sendp data capt --------------
//-----------------------------------------------------
//-- Projet: Nunchuk                                 --
//-- Date:   31-07-2012                              --
//-- Progr:  Manage Nunchuk on I2C bus with servo    --
//-- Auteur: Binon Daniel (UMONS)           --
//-----------------------------------------------------
//-- Commande d'un bras robos (6 servos) avec      --
//-- une manette nunchuk connectée sur le bus I2c  --
//-- Utilisation de deux interruptions pour les    --
//-- impulsions des servos moteurs.          --
//-- Utilisation de l'interruption série pour la   --
//-- reception des commandes de déplacement du bras  --
//-- via le port série après appuis sur le bouton.   --
//--                           --
//----------------------------------------------------- 

#include <16F887.h>

#device ADC=8
#fuses NOWDT, HS, NOPUT, NOPROTECT, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NODEBUG
#use delay(clock=20000000)
#use rs232(baud=57600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=RS232)
#use I2C(master, sda=PIN_C4, scl=PIN_C3, FORCE_HW) 

#include "LCD420_S3.c"

/* definition pour les servos */
#define nb_servos 6
#define vitesse 2

#define servo0(x) output_bit(pin_d7,x);       // Base rotative
#define servo1(x) output_bit(pin_d6,x);       // Inclinaison section 1 bras 
#define servo2(x) output_bit(pin_d5,x);       // Inclinaison section 2 bras 
#define servo3(x) output_bit(pin_d4,x);       // Inclinaison section 3 bras 
#define servo4(x) output_bit(pin_d3,x);       // Rotation Pince 
#define servo5(x) output_bit(pin_d2,x);       // ouverture/fermeture Pince

#define PULSE_ON  1
#define PULSE_OFF 0

/* on défini notre consigne à 1,5ms */
/* 5000 -> 1ms et 10800 -> 2,2ms */
/* 5800 / 256 = 22,6 --> 22 */
#define consigne 7200               // consigne sur milieu de course des servos
#define servocode(x) (((long)x*22)+5000)      // encodage deplacement servo
#define servdecod(x) ((x-5000)/22)
/* definition pour la Nunchuk */

#define nundecode(v) ((v^0x17)+0x17)        // decodage valeur de la Nunchuk

/* definition pour les IO de la carte Simius v3 */

#define led pin_a4
#define ledon output_hight(led);
#define ledoff  output_low(led);
#define Button PIN_E2

/* definition pour l'interface serie */

#define BUFFER_SIZE 16                // Buffer maxi de lecture RS232
#define bkbhit (next_in!=next_out)
#define DATA_HEAD 0xFF

/* definition es variables */
volatile byte s_actif,cpt,x,c;

/* 6 lignes (6 servos) et 2 colonnes (état du pulse et consigne) */
volatile int16 tab[6][2];                 // table de commande des servos
volatile byte tab_dep[6];                 // table de déplacement des servos
// consignes servos:     120,120,90,50,80,2
int16 const tabcons[6] ={120,120,90,50,80,2};     // table de consigne des servos
Byte CONST nunchuk = 0xA4;                  // Adresse Nunchuck

BYTE buffer[BUFFER_SIZE];
BYTE next_in = 0;
BYTE next_out = 0;

int joy_x_axis;
int joy_y_axis;
int16 accel_x_axis; 
int16 accel_y_axis;
int16 accel_z_axis; 
int1 z_button;
int1 c_button;

/***********************************************************
 * fonction qui va refléter les changements de "tab" sur   *
 * les pins de chaque servo                                *
 ***********************************************************/
void maj_servos(void)                   // mise a jour des servos
{
  /* va mettre chaque servo à l'état haut ou bas
   * selon l'état stocké dans le tableau */
  servo0(tab[0][0]);
  servo1(tab[1][0]);
  servo2(tab[2][0]);
  servo3(tab[3][0]);
  servo4(tab[4][0]);
  servo5(tab[5][0]);
}

#int_timer1         // Timer regle sur 13ms mais nous avons besoin de pulse de 1ms à 2.2ms
void pwm_isr() {

    /* on met le servo actif à l'état bas (PULSE_OFF = 0) */
  tab[s_actif][0] = PULSE_OFF;
  /* on change le servo actif */
  if (s_actif < (nb_servos - 1))
    s_actif++;
  else
    s_actif = 0;
  maj_servos();
}

#int_timer2         // Timer regle sur 20ms
void tim2_isr() {

    /* on met le servo suivant à l'état haut (PULSE_ON = 1) */
    tab[s_actif][0] = PULSE_ON;
    /* le timer va se déclencher à la fin de l'impulsion */
    set_Timer1(65535 - tab[s_actif][1]);
    /* on apelle la fonction maj_servos pour refléter les changements */
    maj_servos();
}  

#int_rda                      // interruption serie
void serial_isr() {
   int t;

   buffer[next_in]=getc();              // remplissage du buffer
   t=next_in;
   next_in=(next_in+1) % BUFFER_SIZE;
   if(next_in==next_out)
     next_in=t;                         // Buffer full !!
   c=1;                       // activation commande serie
}

BYTE bgetc() {                    // lecture du buffer serie
   BYTE c;

   while(!bkbhit) ;                 // tant que le buffer n'est pas vide
   c=buffer[next_out];                // lecture du buffer
   next_out=(next_out+1) % BUFFER_SIZE;       // mise a jour du pointeur
   return(c);
}

void initialisation() {

  cpt=0;
  x=0;
  c=0;
  s_actif = 0;
  lcd_init();                   // Initialisation du Display
    lcd_putc("\fNunchuk & Servos\n");
  lcd_putc("(C)UMONS - 2012");
  cursor(0);                    // curseur OFF
  delay_ms(2000); 
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
  setup_timer_2(T2_DIV_BY_16,103,10);
  enable_interrupts(INT_TIMER2);
  enable_interrupts(INT_TIMER1);
  enable_interrupts(int_rda);
  enable_interrupts(GLOBAL);
  set_Timer1(65535 - tab[0][1]);          // Initialisation du timer avec la consigne de depart

}

void init_servos() {                // initialisation des servos sur le consigne de depart

  byte i;
  for (i = 0; i < nb_servos; i++)
  {
    tab[i][0] = PULSE_OFF;
    tab[i][1] = servocode(tabcons[i]);      // traduction de la valeur pour le timer
    tab_dep[i] = tabcons[i];
    delay_ms(200);
  }
}

void init_nunchuk()                 // initialisation de la nunchuk
{
  i2c_start();
    i2c_write(nunchuk);
    i2c_write(0x40);
    i2c_write(0x00);
    i2c_stop();
}

void lect_nunchuk()                 // lecture de la nunchuk
{
  byte c;
  int outnun[6]={0,0,0,0,0,0};

  i2c_start();
    i2c_write(nunchuk);
    i2c_write(0x00);
    i2c_stop();
  
  delay_ms(3);
    i2c_start();
  i2c_write(nunchuk | 1);
  for (c=0;c<5;++c) outnun[c] = nundecode(i2c_read(1));
  outnun[5] = nundecode(i2c_read(0));
    i2c_stop();

  joy_x_axis = outnun[0];             // a partir de ce point -> decodage nunchuk
  joy_y_axis = outnun[1];
  accel_x_axis = outnun[2]; 
  accel_x_axis = accel_x_axis * 4;
  accel_y_axis = outnun[3];
  accel_y_axis = accel_y_axis * 4;
  accel_z_axis = outnun[4];
  accel_z_axis = accel_z_axis * 4;  
  z_button = 0;
  c_button = 0;
  if (outnun[5] & 1) z_button = 1;
  if ((outnun[5] >> 1) & 1) c_button = 1;
  if ((outnun[5] >> 2) & 1) accel_x_axis += 2;
  if ((outnun[5] >> 3) & 1) accel_x_axis += 1;
  if ((outnun[5] >> 4) & 1) accel_y_axis += 2;
  if ((outnun[5] >> 5) & 1) accel_y_axis += 1;
  if ((outnun[5] >> 6) & 1) accel_z_axis += 2;
  if ((outnun[5] >> 7) & 1) accel_z_axis += 1;
}

void aff_nunchuk(){

  printf(lcd_putc,"\fJX%3u Y%3u C%u Z%u\n",tab_dep[4],tab_dep[5],c_button,z_button);
  printf(lcd_putc,"AX%3u Y%3u Z%3u ",tab_dep[0],tab_dep[1],tab_dep[2]);
}

boolean bouton_release() {
  char x=0;
  
  if (input(button)) {
    delay_ms(15);
    if (input(button)) x=1;
    else x=0;
  }
  else x=0;
  return(x);
}
//------------------------xxx---------------------
boolean bouton_press() {        // test si le bouton de la carte est presse
  char x=0;
  
  if (!input(button)) {
    delay_ms(15);
    if (!input(button)) x=1;
    else x=0;
  } 
  else x=0;
  while (!bouton_release());
  return(x);
}

void servo(byte serv, byte pos){    // commande de configurattion d'un servo
  long val1;

  val1=servocode(pos);
  tab[serv][1] = val1 ;
}

void conf_servo() {           // procedure de configuration des servos
  byte i;

  for (i = 0; i < nb_servos; i++)
    servo(i,tab_dep[i]);
}

void lec_joy(){             // lecture des joysticks pour les deplacements

  byte x,y;
  y=joy_y_axis;
  x=joy_x_axis;

  //condition sur le déplacement du joystick en y
  if ((128+y)<129 && (128+y)>50 && tab_dep[4]<252 ) tab_dep[4]=tab_dep[4]+vitesse+3;
  else if( y<100 && y>0 && tab_dep[4]>8 ) tab_dep[4]=tab_dep[4]-vitesse-3;

  if ( 118+x>80 && 118+x<120 && tab_dep[5]<252  ) tab_dep[5]=tab_dep[5]+vitesse+3;
  else if( x<100 && x>0 && tab_dep[5]>8 ) tab_dep[5]=tab_dep[5]-vitesse-3;
}

/* Plage de deplacement des accelerometres de la nunchuk */
/*            Min             Max                        */
/*   Ax  -->  312 <---------> 720   deplacement de 408   */
/*   Ay  -->  303 <---------> 718   deplacement de 415   */
/*   Az  -->  309 <---------> 730   deplacement de 421   */

void lec_acc(){             // lecture des accelerometres pour les deplacements

  int16 xx,yy,zz;
  xx=accel_x_axis;
  yy=accel_y_axis;
  zz=accel_z_axis;

  if (c_button == 0 && z_button == 1) {  // C => xx et yy commande servo rotation + elevation servo 1

    if (xx<700 && xx>550 && tab_dep[0]<254 ) tab_dep[0]=tab_dep[0]+vitesse;
    else if( xx<450 && xx>320 && tab_dep[0]>5 ) tab_dep[0]=tab_dep[0]-vitesse;

    if (yy<700 && yy>550 && tab_dep[1]<254 ) tab_dep[1]=tab_dep[1]+vitesse;
    else if( yy<450 && yy>320 && tab_dep[1]>5 ) tab_dep[1]=tab_dep[1]-vitesse;
  }
  if (z_button == 0 && c_button == 1) {  // Z => xx et yy commande elevation servo 2 + elevation servo 3
 
    if (xx<700 && xx>550 && tab_dep[2]<254 ) tab_dep[2]=tab_dep[2]+vitesse;
    else if( xx<450 && xx>320 && tab_dep[2]>5 ) tab_dep[2]=tab_dep[2]-vitesse;

    if (yy<700 && yy>550 && tab_dep[3]<254 ) tab_dep[3]=tab_dep[3]+vitesse;
    else if( yy<450 && yy>320 && tab_dep[3]>5 ) tab_dep[3]=tab_dep[3]-vitesse;
  }
  if (c_button == 0 && z_button == 0) {  //// C + Z => Initialisation des servos (depart initial)
    
    init_servos();    
  }
}

void send_ok(){             // Envoie un OK a l'application

  delay_ms(10);           // leger temps d'attente pour ne pas recevoir trop vite la sequence suivante
  putc(255);              // envoi vers le port serie de l'OK       
}

void cmdservo(byte serv,byte in,byte out) {
  byte i;
    
  if (out > in) {
    for (i = in; i < out; i++) {    // boucle du point initiale jusqu'au point d'arrivee
      servo(serv,i);          // mise a jour de la position du servo
      delay_ms(5);          // temps pour reduire les accoup
    }
  }
  else {                  // si les points sont inverses on change de direction
    i=in;                 
    while (i != out){
      servo(serv,i);          // mise a jour de la position du servo
      i--;              
      delay_ms(5);          // temps pour reduire les accoup
    }
  }
  send_ok();              // commande terminee
}

/*  Commandes des servo par le biais du port serie                  */
/*  FF,FE,00  -->  lance le reset des servos + consigne de depart   */
/*  FF,FF,00  -->  relache la commande des servos par le port serie */
/*  FF,Numero servo,Consigne  -->  Commande d'un servo              */
/*  Numeros de servo de 0 a 6   --   consigne de 0 a 255            */

void lect_cmds(){                 // reception des commandes par le port serie
  byte data[4],tmp;

  while (bkbhit) {
    data[0]=bgetc();
    if (data[0] == DATA_HEAD) {         // lecture du debut de la trame (0xFF)
      data[1]=bgetc();            // lecture du numero du servo
      data[2]=bgetc();            // lecture de la consigne a appliquer au servo
      switch (data[1]) {

        case 0:               // Commande rotation base (serv 0)
          tmp = servdecod(tab[0][1]);
          cmdservo(0,tmp,data[2]);
          break;

        case 1:               // Commande inclinaison 1 bras (serv 1)
          tmp = servdecod(tab[1][1]);
          cmdservo(1,tmp,data[2]);
          break;

        case 2:               // Commande inclinaison 2 bras (serv 2)
          tmp = servdecod(tab[2][1]);
          cmdservo(2,tmp,data[2]);
          break;

        case 3:               // Commande rotation bras (serv 3)
          servo(3,data[2]);
          send_ok();
          break;
          
        case 4:               // Commande rotation Pince (serv 4)
          servo(4,data[2]);
          send_ok();
          break;

        case 5:               // Commande ouverture/fermeture Pince (serv 5)
          servo(5,data[2]);
          send_ok();
          break;

        case 0xFF:                // Commande ouverture/fermeture Pince (serv 5)
          send_ok();
          c=0;
          break;

        case 0xFE:
          init_servos();    
          send_ok();
          break;

        default :
          while (bkbhit) {
            bgetc();                    // vidage buffer si non OK
          }
          break;
      } 
    }  
  }
  lcd_putc("\fCmds en RS232  \n");
  printf(lcd_putc,"Ser:%2u cons:%3u \n",data[1],data[2]);
}

void cmd_servo(byte cmd){                 // programme principale

    switch (cmd) {

      case 0:                     // choix 0 - bras controle par la nunchuk
        lect_nunchuk();               // lecture de la nunchuk
        lec_joy();                  // lecture des joysticks
        lec_acc();                  // lecture des acceleromtres
        aff_nunchuk();                // affichage des parametres
        conf_servo();               // application des parametres aux servos
        break;

      case 1:                     // choix 1 - bras controle via port serie
        lect_cmds();
        break;

      default :
        break;
    }
}

void main(void)
{

  init_servos();                // Initialisation table des Servos + consignes de depart
  initialisation();             // initailisation de la carte Simius & affichage
  init_nunchuk();               // Initialisation de la manette nunchuk
  while (true) {
    if (bouton_press()) c=(c+1) % 2;    // choiox entre les deux applications
      cmd_servo(c);
  }

}
