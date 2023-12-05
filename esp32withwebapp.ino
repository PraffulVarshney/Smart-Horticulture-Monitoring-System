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

#define WIFI_SSID "Arjun"
#define WIFI_PASSWORD "11111111"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDZrwJDY7-V8-MgP52l5RcwvcZIE6vcYYk"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smarthorticulturesystem-default-rtdb.asia-southeast1.firebasedatabase.app" 

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
#define DHT11PIN 17
#define switch 16

#define pinbuzz 18

DHT dht(DHT11PIN, DHT11);
int _moisture,sensor_analog;
const int sensor_pin = A3;  /* Soil moisture sensor O/P pin */
const int gas = 0;
int MQ2pin = A7;
int flg=0;
void setup(void){
  Serial.begin(115200);     /* Set the baudrate to 115200*/
  dht.begin();
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".../");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setHumidity(float count){
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "humi/float", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
void setTemperature(float count){
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "temp/float", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
void setMoisture(float count){
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "moisture/float", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}


void loop(void){

    
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

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)){
    setTemperature(temp);
    setHumidity(humi);
    setMoisture(_moisture);
    sendDataPrevMillis = millis();
  }
  delay(1000);
}