#include "Libraries.h"
#include "Wire.h"
#include "SRF02.h"
  
  
  //Serial.begin(9600);
// initialisation des valeurs 
unsigned long clock_begin = 0;
unsigned long clock_end = 0;
int dist =0;

SRF02 sensor(0x70, SRF02_CENTIMETERS);
unsigned long nextPrint = 0;

//Nom et type du servo
Servo SERVOS[NSERVO];

int REAL[NSERVO];
int TARGET[NSERVO];
int SPEEDS[NSERVO];

boolean DIR[NSERVO] = {true,true,true,false,true,true};

unsigned long PrevTime = 0;
const long interval = 1000; //intervale de 50 millisecondes
const char T[][6] = { 0,0,0,0,0,0,
                      50,50,50,50,50,50,
                      80,80,80,80,80,80,
                      50,50,50,50,50,50,
                      0,0,0,0,0,0,
                      0,0,0,100,0,39
                      };
const unsigned int TMAX =  (sizeof(T)/sizeof(char))/6;
unsigned int count = 0;

void loop()
{
  Wire.begin();
  SRF02::update();
  if (millis() > nextPrint)
  {
  Serial.println(sensor.read());
    nextPrint = millis () + 1000;
  }
}

void servoSetup(int num) {
  SERVOS[num].attach(PINS[num]); // Attachement au pin's
  TARGET[num] = INIT[num];
  REAL[num] = INIT[num];
  SPEEDS[num] = 0;
  SERVOS[num].write(INIT[num]);
}

void SetupServoArm() {
  for (int num = 0; num < NSERVO; num++) {
    servoSetup(num);
  }
}

void UDPSetup(){
  WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);
  WiFi.config(local, dns, netmask);
  WiFi.mode(WIFI_STA); 
  WiFi.begin("SEMI_PUBLIC"); //Nom du WIFI 
    
  Serial.begin(115200);

  Udp.begin(UDPPort); //écout de l'UDP PORT
  Serial.printf("écoute @ IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), UDPPort); //affichage
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void UDP_Reception() {
  //if ( WiFi.status() != WL_CONNECTED )  return;
  int packetSize1 = Udp.parsePacket(); //vérification du packet reçu
  // receive incoming UDP packets
  if ( packetSize1 ) { //if the packet was received then:
        //Serial.print(packetSize1);
        //Serial.printf("-");
    if ( Udp.read(packetBuffer, (2 * NSERVO)) > 0 ) {
      for (int num = 0; num < NSERVO; num++) {
        SPEEDS[num] = int(packetBuffer[num]);
        TARGET[num] = int(packetBuffer[num + NSERVO]);
        Serial.print(num);
        Serial.printf(":");
        Serial.print(SPEEDS[num]);
        //Serial.printf(":");
        //Serial.print(TARGET[num]);
        //Serial.printf(" ");
        Serial.println();
      }
      //Serial.println("");
    }
  }
}
  if(dist> 50){
    
    MoveMyArm();
  
  }
// mise à jour de la position du servo selon la vitesse reçu en paramètre
void MoveMyServo(int num) {
  // augmenter la position du moteur en fonction du paramètre de vitesse
  if (SPEEDS[num] = 13) {
    TARGET[num]=REAL[num];
  }
  else if (SPEEDS[num] > 0) {
    if (REAL[num] > TARGET[num]) REAL[num] = ( REAL[num] + TARGET[num] )/2;
    REAL[num] = REAL[num] + SPEEDS[num];
  }
  else {
    // lorsque la vitesse est nulle, diminuer la position du moteur
    REAL[num] = REAL[num] - 1;
    //la position du moteur ne peut pas être inférieure à la valeur initiale
    if (REAL[num] < INIT[num]) REAL[num] = INIT[num];
  }
  
  // envoyer la valeur de position au moteur
  SERVOS[num].write( DIR[num]? REAL[num] : MAX[num]-REAL[num] ); 
  //SERVOS[num].write(REAL[num]);
  //Serial.print(REAL[num]);
  //Serial.printf(" ");
}


//mettre à jour la position de chaque moteur de bras
void MoveMyArm(){
    for (int num = 0; num < NSERVO; num++) {
      MoveMyServo(num);    
  }
}

void loopServoDance() {
  while(count<TMAX){
    unsigned long NowTime = millis();
    unsigned long DanceTime = millis();
    if (NowTime - PrevTime >= interval) {
      PrevTime = NowTime;
      SERVOS[5].write(map(T[count][5],0,100,0,180));
      SERVOS[4].write(map(T[count][4],0,100,40,120));
      SERVOS[3].write(map(T[count][3],0,100,10,100));
      SERVOS[2].write(map(T[count][2],0,100,10,140));
      SERVOS[1].write(map(T[count][1],0,100,0,180));
      SERVOS[0].write(map(T[count][0],0,100,0,180));
      Serial.print(count);
      Serial.printf(" ");
      count++;
      return;
    }
  }
  return;
}





