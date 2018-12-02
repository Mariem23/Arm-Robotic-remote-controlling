void main(void)
{

  init_servos();                // Initialisation table des Servos + consignes de depart
  initialisation();             // initialisation de la carte Simius & affichage
  printf(lcd_putc,"\fWaiting...");      // Attente d'une première trame d'instructions
  delay_ms(1000);
  while (true) 
  {
      lect_cmds();
  }
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
