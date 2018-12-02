/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generated  with TUNIOT FOR ESP8266 Website: Easycoding.tn                                           //
// Found in https://github.com/gowgear/UDP-Android-ESP                                                 //
// And also with the help of Arduino Forum found in:                                                   //
// https://forum.arduino.cc/index.php?topic=6666.0                                                     //
//   Communication Client/serveur      //



#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

unsigned int UDPPort = 2380; // local port to listen on
char packetBuffer[255]; //buffer to hold incoming packet

String readStrings;
char c;
char readchars;
char buf[255];
unsigned int len;


WiFiUDP Udp;

//Ip configuration.
const IPAddress local(11, 11, 11, 11); // ip: the IP address of the device.
const IPAddress dns(11, 11, 11, 11); // dns: the address for a DNS server.
const IPAddress netmask(255, 255, 255, 0); // subnet mask
const IPAddress remote(11,11,11,11); // gateway IP address

// Setup
void setup() {
  WiFi.softAPdisconnect(true); //Disconnect stations from the network established by the soft-AP.
  // It is set to true, so it will switch the soft-AP mode off.
  WiFi.disconnect(true); //Disconnects the WiFi shield from the current network.
  WiFi.config(local, dns, netmask); //WiFi.config(ip, dns, subnet);
  WiFi.mode(WIFI_STA); //Mode Station/Client
  WiFi.begin("MyWiFi"); //Tells the server to begin listening for incoming connections at MyWiFi.
  Serial.begin(115200); //Number of the serial port

  Udp.begin(UDPPort); //listening to the port UDPPort
  Serial.println("Beggining of the mensages");         
}

void loop() {
  ////////// SENDING MENSAGES
  readStrings=""; //Cleaning the last string
  while (Serial.available()) { //serial is available?
     delay(10);
     // changing char to string:
     if (Serial.available() >0) {
       char c = Serial.read();
       readStrings += c;}
  }
  len = readStrings.length();  //length of the string
  if (len >0) {
     readStrings.toCharArray(buf, len+1); //changing the string to an array of char
     Udp.beginPacket(remote, UDPPort); // Starts the connection to write UDP data to the remote connection. Syntax: UDP.beginPacket(remoteIP, remotePort); 
     Udp.write(buf); // Writes UDP data to the remote connection. Must be wrapped between beginPacket() and endPacket(). Syntax: UDP.write(message); message: the outgoing message (char).
     Udp.endPacket();
     
     Serial.printf("Sending: %s\n", buf); // print my mensage    
    
    }


  ////////// RECEIVING MENSAGES
   
  int packetSize1 = Udp.parsePacket(); // Checks for the presence of a UDP packet, and reports the size.

  // receive incoming UDP packets
  if (packetSize1) { //if the packet was received then:
    int len = Udp.read(packetBuffer, 255); // Udp.read() Reads UDP data from the specified buffer. If no arguments are given, it will return the next character in the buffer.      
    if (len > 0) { // if the size is bigger then 0 then:
      packetBuffer[len] = 0; //at the end of the packetBuffer it puts a 0. This is needed to identify the end of the packet for the Serial.printf().
      Serial.printf("Recieving: %s\n", packetBuffer);
    }
  }
}
