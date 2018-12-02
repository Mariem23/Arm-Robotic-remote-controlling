// program to test LED
// Mariem Fattoum

void setup() {
 pinMode(2, INPUT); //pin d'entrée
 pinMode(13, OUTPUT); //pin de sortie 
}

void loop() {
  int a; //déclaration de variable d'entrée 
  int b; //déclaration variable de sortie 
  a = digitalRead (2); // lecture de la valeur d'entrée
  b = 1; 
  if (a==b)
  {digitalWrite (13, HIGH); //si les deux valeurs sont égaux , on allume la LED
  }
  else
  digitalWrite (13, LOW); // sinon on l'atteint
}
