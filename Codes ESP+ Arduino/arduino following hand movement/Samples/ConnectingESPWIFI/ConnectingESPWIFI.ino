#include <ESP8266WiFi.h>
void setup()
{
  Serial.begin(4);
  Serial.println();

  WiFi.begin("SEMI_PUBLIC", "microelecpub");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
