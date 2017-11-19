/*
  YunSensorMote

   A Sensor Mote using Arduino Yun that comunicates vía Wifi 
   through REST calls using the Bridge Library.

   It use a JSONp library to return json objects with the raw data.
   
     The Circuit:
  * Arduino A0 connected to Vout signal pin of Analog Temperature Sensor.
  * Arduino A1 connected to Vout signal pin of Humidity DHT11 Sensor.
  * Arduino A2 connected to Vout signal pin of LDR Sensor.
  * Arduino A3 connected to a 10K Pot that will simulate any other analog sensor.
  * Arduino D8 connected to Relay Module
  * Arduino I2C SDA/SCL connected to Adafruit's MPL3115A2 Sensor Module

  The Date:
  created 06 May 2017

  The Author:
  by Alvaro Saburido Rodriguez
  alvaro.saburido@gmail.com

  This example code is in the public domain. Please share and enjoy
*/
#include <Arduino.h>
#include <Wire.h>
// #include <Adafruit_MPL3115A2.h>
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>

#define MOTEID 1 // Mote ID
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTPIN 2     // what digital pin we're connected to
#define beta 4090 //the beta of the thermistor
// Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2(); //I2C
int tempPin = A0; // Select the input from Analog Temperature Sensor
int humidityPin = 2; // Select the input from Humidity DHT11 Sensor
int ldrPin = A2; // Select the input from Linear LDR Sensor
int adcPin = A3; // Select the input from Custom Analog Sensor(POT).
int relayPin = 8; // Output to Relay Module.
int ledPin = 13;

int groupId = 2;
int temp = 0;
float humidity = 0;
int ldr = 0;
int adc = 0;
int relay = 0;
float altm = 0;
// float pressure = 0;

DHT dht(DHTPIN, DHTTYPE);

BridgeServer server;
// Inside the brackets, 200 is the size of the pool in bytes.
// If the JSON object is more complex, you need to increase that value.
StaticJsonBuffer<100> jsonBuffer;
// It's a reference to the JsonObject, the actual bytes are inside the
// JsonBuffer with all the other nodes of the object tree.
// Memory is freed when jsonBuffer goes out of scope.
JsonObject& dataJson = jsonBuffer.createObject();
JsonObject& sensorJson = jsonBuffer.createObject();

void getTemp() {
  // Get raw value of Temperature
  temp = analogRead(tempPin);
  Serial.print("Temp: ");
  Serial.println(temp);
   //read thermistor value 
  long a =1023 - temp;
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  Serial.println(tempC);
}

void getLDR() {
  // Get raw value of MPL3115A2 Sensor
  ldr = analogRead(ldrPin);
  Serial.print("LDR: ");
  Serial.println(ldr);
}

void getHumidity() {
  // Get raw value of Humidity
  humidity = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humidity);
}
void getAdc() {
  // Get raw value of Humidity
  adc = analogRead(adcPin);
  Serial.print("Adc: ");
  Serial.println(adc);
}

void setup() {
  // put your setup code here, to run once:
  // Bridge startup
  pinMode(13, OUTPUT);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600); 

  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}
void dataCommand(BridgeClient client) {
    getTemp();
    getHumidity();
    getLDR();
    getAdc();

    dataJson["moteId"] = MOTEID;
    dataJson["groupId"] = groupId;
    dataJson["temp"] = temp;
    dataJson["humidity"] = humidity;
    dataJson["ldr"] = ldr;
    /* dataJson["pressure"] = pressure;
    dataJson["altitude"] = altm; */
    dataJson["adc"] = adc;
    dataJson["relay"] = relay;
    // Send feedback to client
    dataJson.printTo(client);
}


void tempCommand(BridgeClient client) {
    getTemp();
    sensorJson["sensor"] = "temperature";
    sensorJson["value"] = temp;
    // Send feedback to client
    sensorJson.printTo(client);
    
    // Update datastore key with the current pin value
    String key = "A0";
    Bridge.put(key, String(temp));
}
void humidityCommand(BridgeClient client) {
    getHumidity();
    sensorJson["sensor"] = "humidity";
    sensorJson["value"] = humidity;
    // Send feedback to client
    sensorJson.printTo(client);
    
    // Update datastore key with the current pin value
    String key = "A1";
    Bridge.put(key, String(humidity));
}
void barometricCommand(BridgeClient client) {
    /* getPressure();
    sensorJson["sensor"] = "pressure";
    sensorJson["value"] = pressure;
    // Send feedback to client
    sensorJson.printTo(client); */
}
void altitudeCommand(BridgeClient client) {
    /* getAltitude();
    sensorJson["sensor"] = "altitude";
    sensorJson["value"] = altm;
    // Send feedback to client
    sensorJson.printTo(client); */
}
void ldrCommand(BridgeClient client) {
    getLDR();
    sensorJson["sensor"] = "ldr";
    sensorJson["value"] = ldr;
    // Send feedback to client
    sensorJson.printTo(client);
    
    // Update datastore key with the current pin value
    String key = "A2";
    Bridge.put(key, String(ldr));
}
void adcCommand(BridgeClient client) {
    getAdc();
    sensorJson["sensor"] = "adc";
    sensorJson["value"] = adc;
    // Send feedback to client
    sensorJson.printTo(client);
    
    // Update datastore key with the current pin value
    String key = "A3";
    Bridge.put(key, String(adc));
}
void relayCommand(BridgeClient client) {
    int value = client.parseInt();
    digitalWrite(relayPin, value);
    relay = value;
    sensorJson["sensor"] = "relay";
    sensorJson["value"] = value;
    // Send feedback to client
    sensorJson.printTo(client);
    
    // Update datastore key with the current pin value
    String key = "D8";
    Bridge.put(key, String(relayPin));
}
void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');
  client.println("Status: 200");
  client.println("Access-Control-Allow-Origin: *");   
  client.println("Access-Control-Allow-Methods: GET");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  // is "connect" command?
  if (command == "connect") {
    dataJson["status"] = "OK";
    dataJson.printTo(client);
  }
  // is "data" command?
  if (command == "data") {
     dataCommand(client);
  }
  // is "temperature" command?
  if (command == "temp") {
    tempCommand(client);
  }
  // is "humidity" command?
  if (command == "humidity") {
    humidityCommand(client);
  }
  // is "ldr" command?
  if (command == "ldr") {
    ldrCommand(client);
  }
  // is "barometric" command?
  if (command == "barometric") {
    barometricCommand(client);
  }
  // is "altitude" command?
  if (command == "alditude") {
    altitudeCommand(client);
  }
  // is "adc" command?
  if (command == "adc") {
    adcCommand(client);
  }
  // is "relay" command?
  if (command == "relay") {
    relayCommand(client);
  }
}
void loop() {
  
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(200); //wait a while ;
}



