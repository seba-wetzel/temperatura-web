#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>

#define Pin 2
OneWire ourWire(Pin);
DallasTemperature sensors(&ourWire);
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "arduino.electro-ar.tk";
IPAddress ip(192,168,0,177);
EthernetClient client;
float temperatura = 0;

void setup() {
  delay(500);
  sensors.begin();
  Serial.begin(9600);
  while (!Serial) {
    ;   
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(500);

}


void loop() {
  sensors.requestTemperatures(); //Prepara el sensor para la lectura 
  temperatura = sensors.getTempCByIndex(0);
  Serial.println(temperatura);
  delay(500);
  Serial.println("connecting...");
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.print("GET /iot.php?valor="); // Enviamos los datos por GET
    client.print(temperatura);
    client.println(" HTTP/1.0");
    client.println("User-Agent: Arduino 1.0");
    client.println();

  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  client.stop();
  client.flush();
  delay(2000);
}






