#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

unsigned int UDPPort = 2380; // local port to listennon
char packetBuffer[255]; //buffer to hold incoming packet

String readStrings;
char MsgSending;
char readchars;
char PackageSended[255];
unsigned int len;

WiFiUDP Udp;

//////////////// Ip configuration
const IPAddress local(11, 11, 11, 11); // ip: the IP address of the device.
const IPAddress dns(11, 11, 11, 11); // dns: the address for a DNS server.
const IPAddress netmask(255, 255, 255, 0); // subnet mask
const IPAddress remote(11,11,11,11); // gateway IP address

//////////////// Setup of Wifi Station
void setupWIFI() {
  WiFi.softAPdisconnect(true); //Disconnect stations from the network established by the soft-AP.
  // It is set to true, so it will switch the soft-AP mode off.
  WiFi.disconnect(true); //Disconnects the WiFi shield from the current network.
  WiFi.config(local, dns, netmask); //WiFi.config(ip, dns, subnet);
  WiFi.mode(WIFI_STA); //Mode Station/Client
  WiFi.begin("MyWiFi"); //Tells the server to begin listening for incoming connections at MyWiFi.

  Udp.begin(UDPPort); //listening to the port UDPPort      
}

//////////////// SENDING MENSAGES
void SendingMensage(){
  // Sequence for reading message from the serial port //
  readStrings=""; //Cleaning the last string
  while (Serial.available()) {
     delay(10);
     if (Serial.available() >0) { // changing char to string
       char MsgSending = Serial.read();
       readStrings += MsgSending; //String of the message on the serial port
     }
  }
  
  // Sequence for sending message with the Wifi //
  len = readStrings.length();  //length of the string
  if (len >0) {
     readStrings.toCharArray(PackageSended, len+1); //changing the string to an array of char
     Udp.beginPacket(remote, UDPPort); // Starts the connection to write UDP data to the remote connection. Syntax: UDP.beginPacket(remoteIP, remotePort); 
     Udp.write(PackageSended); // Writes UDP data to the remote connection. Must be wrapped between beginPacket() and endPacket(). Syntax: UDP.write(message); message: the outgoing message (char).
     Udp.endPacket();

     Serial.printf("You: %s\n", PackageSended); // print my mensage      
  }
}

//////////////// RECEIVING MENSAGES
void ReceivingMensage (){
  int packetSize1 = Udp.parsePacket(); // Checks for the presence of a UDP packet, and reports the size.

  // receive incoming UDP packets
  if (packetSize1) { //if the packet was received then:
    int len = Udp.read(packetBuffer, 255); // Udp.read() Reads UDP data from the specified buffer. If no arguments are given, it will return the next character in the buffer.      
    if (len > 0) { // if the size is bigger then 0 then:
      packetBuffer[len] = 0; //at the end of the packetBuffer it puts a 0. This is needed to identify the end of the packet for the Serial.printf().
      Serial.printf("Resp: %s\n", packetBuffer);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                SETUP                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  setupWIFI();
  Serial.begin(115200); //Number of the serial port
  Serial.println("Beggining of the mensages");         
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                LOOP                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  SendingMensage();
  ReceivingMensage();
}
