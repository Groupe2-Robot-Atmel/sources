/*
  WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort

 Circuit:
 * WiFi shield attached

 created 30 December 2012
 by dlf (Metodo2 srl)

 */


#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
const int ledPin    = PIN_LED_13;
char ssid[] = "Wifi_Arduino"; //  your network SSID (name)
char pass[] = "aarduino";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
IPAddress ip(255, 255, 255, 255);
IPAddress ipLead;
bool lead = true;
int count = 1;

unsigned int localPort = 8888;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "Blabla";       // a string to send back

WiFiUDP Udp;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(1000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  ipLead = WiFi.localIP();
  Udp.begin(localPort);
  Udp.beginPacket(ip, localPort);
  Udp.write("Je suis le leader");
  Udp.endPacket();
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    String message = packetBuffer;
    message.trim();
    if(message == "Je suis le leader" ){
      ipLead = Udp.remoteIP();
      Serial.print("Nouveau leader : ");
      Serial.println(ipLead);
      lead = false;
    }
  }
  if(lead){
      digitalWrite(ledPin, LOW);
    }
  else {
       digitalWrite(ledPin, LOW);
       delay(1000);
       digitalWrite(ledPin, HIGH);
       delay(1000);
    }
  //Serial.println(ipLead);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}




