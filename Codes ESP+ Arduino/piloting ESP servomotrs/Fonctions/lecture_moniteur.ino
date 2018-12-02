char val; // variable to receive data from the serial port
int ledPin = 13;

void setup() {
     Serial.begin(9600);      // opens serial port, sets data rate to 9600 bps
       pinMode(ledPin, OUTPUT); 
}

void loop() {

 if( Serial.available() )       // if data is available to read
 {
   val = Serial.read();         // read it and store it in 'val'
   Serial.print("I received: ");
   Serial.println(val);
 }
 if( val == 'A' ) digitalWrite(ledPin, HIGH);  // turn ON the LED
 if( val == 'B' ) digitalWrite(ledPin, LOW);   // turn OFF the LED
 delay(100);                                   // wait 100ms
}
