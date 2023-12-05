#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include<Firebase_ESP_Client.h>
#include "DHT.h"

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <HTTPClient.h>
#include <ThingSpeak.h>

const char* ssid = "Arjun";
const char* password = "11111111";
unsigned long Channel_ID=2112679;
const char* Channel_API_KEY="D47UMEB2S9JCVOOJ";
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
#define DHT11PIN 17
#define switch 16

#define pinbuzz 18
// Domain Name with full URL Path for HTTP POST Request
// Service API Key

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Set timer to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Timer set to 10 seconds (10000)
unsigned long timerDelay = 10000;
WiFiClient client;
DHT dht(DHT11PIN, DHT11);
int _moisture,sensor_analog;
const int sensor_pin = A3;  /* Soil moisture sensor O/P pin */
const int gas = 0;
int MQ2pin = A7;
int flg=0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".../");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
  signupOK = true;
  // Random seed is a number used to initialize a pseudorandom number generator
  randomSeed(analogRead(33));
  ThingSpeak.begin(client);
  Serial.begin(115200);     /* Set the baudrate to 115200*/
   
    pinMode(switch , OUTPUT);
    pinMode(pinbuzz , OUTPUT);
}

void loop() {
  //Send an HTTP POST request every 10 seconds
  float sensorValue;
  sensorValue = analogRead(MQ2pin); // read analog input pin 0
    //Serial.print(sensorValue);
    
  if(sensorValue >= 2000){
    flg=1;
  }
  else flg=0;

  //Serial.println(flg);
  if(flg==1){
    Serial.println("SMOKE DETECTED");    
    digitalWrite(pinbuzz,HIGH);
    
    delay(5000);
    digitalWrite((pinbuzz), LOW);
  }
  else
  {
    digitalWrite(pinbuzz, LOW);
  }
  sensor_analog = analogRead(sensor_pin);
  Serial.print("smoke level = ");
  Serial.println(sensorValue);
 _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  Serial.print("Moisture = ");
  Serial.print(_moisture);  /* Print Temperature on the serial window */
  Serial.println("%");
  Serial.println(sensor_analog);
  Serial.print("switch ");
  Serial.println(switch);
  // digitalWrite(pinbuzz,HIGH);
  // delay(5000);
  digitalWrite(pinbuzz, LOW);
  // delay(5000);

  if(_moisture<40) digitalWrite(switch, HIGH);
  else digitalWrite(switch, LOW);
  // Serial.println(switch);
  //delay(5000);
  
  
  // Serial.println(switch);
  //delay(5000);
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);

  if ((millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)){
    // setTemperature(temp);
    // setHumidity(humi);
    // setMoisture(_moisture);
    ThingSpeak.setField(1,temp);
    ThingSpeak.setField(2,humi);
    ThingSpeak.setField(3,_moisture);
    ThingSpeak.writeFields(Channel_ID,Channel_API_KEY);
    sendDataPrevMillis = millis();
  }
  delay(1000);
  //counter++;
  
  
}