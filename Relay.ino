
#include <PubSubClient.h>
#include <WiFiNINA.h> /


char ssid[] = "Livebox-ORBI";        
char pass[] = "****";  



WiFiClient wifiClient;
PubSubClient client(wifiClient);

const char broker[] = "192.168.1.97";
int        port     = 1883;
const char topic[]  = "bedroom/outlet";
const char* mqttUser = "brokerRasp";
const char* mqttPassword = "brokerRasp";

long loopNumber = 0;
String incomingMessage = "";
int relay_pin = 8;
int led_pin = 13;




void connectWifi(){
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println("");
  Serial.println("You're connected to the network");
  Serial.println();
}



void connectMQTT(){
  //MQTT
  client.setServer(broker, port);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    
    if (client.connect("ArduinoClient", mqttUser, mqttPassword )) {
      Serial.println("connected!");  
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
    
    client.subscribe(topic);
  }
}


void setup() {
  //Initialize serial and wait for port to open:
 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  connectWifi();

  connectMQTT();
pinMode(relay_pin,OUTPUT);
  pinMode(led_pin,OUTPUT);  
digitalWrite(led_pin,HIGH);
}


//Stuff to deal with the MQTT incoming messages
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.println();
  Serial.println();
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  incomingMessage = "";
     
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    incomingMessage += (char)payload[i];
  }
 
  Serial.println();
  Serial.println("The total message is:" + incomingMessage);
  Serial.println("---------------------------------------------------");

  if (incomingMessage == "On"){
    digitalWrite(relay_pin,HIGH);;
  }else if (incomingMessage == "Off"){
    digitalWrite(relay_pin,LOW);
  }
}


void loop() {
  client.loop();

  //Are we connected to MQTT? If not, reconnect
  if(!client.connected()){
    Serial.println("MQTT connection lost - trying again.");
    connectMQTT();
  }
}
