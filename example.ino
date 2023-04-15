#include "config.h"

#include <WiFi.h> //Import two new library
#include <PubSubClient.h>

// #define WIFI_SSID "Wokwi-GUEST" //WIFI SSID NAME
// #define WIFI_PW "" //WIFI PASSWORD
// #define MQTT_BROKER "soldier.cloudmqtt.com" //MQTT BROKER IP
// #define MQTT_PORT 16143 //MQTT BROKER PORT
#define MQTT_TOPIC "LED" //TOPIC NAME, NAME IT YOURSELF

WiFiClient espClient; // ESP8266 Wifi Client
PubSubClient client(espClient); // MQTT Client using the Wifi-Client

bool state = false;
int bad_tryes_connect = 0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT); // Set up pin IO modes
  pinMode(23, OUTPUT);
  pinMode(22, INPUT);

  digitalWrite(LED_BUILTIN, HIGH); // Turn off the lights

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PW); // Start Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) { // Waiting for connection success
    delay(250);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  delay(100);
  digitalWrite(LED_BUILTIN, LOW); //Turn on the light to indicate Wi-Fi success
  Serial.println("-------------------------");
  Serial.println("Hello from ESP8266");
  Serial.print("Connected to hotspot: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address is: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------");
  // client.setSocketTimeout(60);
  // client.setKeepAlive(90);
  client.setServer(MQTT_BROKER, MQTT_PORT); //Start MQTT connection
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266_Receive", "xsxvxcnt", "xrV4AvRONNWg" )) { //Sign in to the broker
      Serial.println("Connected to broker");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(MQTT_TOPIC);
}

void loop() {
  
  if (!client.connected()) {
    Serial.print("!");
    if (client.connect("ESP8266_Receive", "xsxvxcnt", "xrV4AvRONNWg" )) { //Sign in to the broker
      Serial.println("Connected to broker");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      bad_tryes_connect++;
      if(bad_tryes_connect > 3){
        ESP.restart();
      }
      delay(2000);
    }
  } else {
    client.loop();
    if(digitalRead(22)){
      while(digitalRead((22)));
      publish_button_value("LED", state);
    }
  }
  delay(10);
}

//_____________________________________________________________________//
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: "); // Print out the topic
  Serial.println(topic);

  Serial.print("Message:"); //Print Received message
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  if(topic = "LED"){
    state = bool((char)payload[0] - '0');
    digitalWrite(23, state);
  }
}


bool publish_button_value(char* topic, bool value){
  if(value){
    return client.publish(topic, "0");
  }else{
    return client.publish(topic, "1");
  }
}