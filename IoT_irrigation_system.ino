#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
String apiKey = "IH25F17BF113FANI";
const char *ssid =  "PVJ";
const char *pass =  "spidy023";
const char* server = "api.thingspeak.com";

WiFiClient client;


//LCD 
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

//servo
Servo myservo;
int pos = 0;

//DHT
DHT dht(D4, DHT11);
const int trigPin = 12;
const int echoPin = 14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;
const int sensor_pin = A0;  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */

//rainfall
int sensorPin = D7;
int sensorValue2=0;


void setup() {
  Serial.begin(115200); // Starts the serial communication
  dht.begin();// begin DHT
   Wire.begin();//I2C begin
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
myservo.attach(15);//sdrvo
lcd.init();//lcd initialize
 lcd.backlight();//ON backlight
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  pinMode(16,OUTPUT);
}

  

void loop() {

  float h=dht.readHumidity();
  float t=dht.readTemperature();
  //ULTRASONIC
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  delay(1000);

  if(distanceCm<10){
    digitalWrite(16,HIGH);
  }
  else{
    digitalWrite(16,LOW);
  }
  //SOIL MOISTURE
  float moisture_percentage;
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin) / 1023.00) * 100.00 ) );
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  delay(1000);

  if(moisture_percentage<30){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
       myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
  }
  }
  //DHT11
  delay(1000);
  Serial.println("Temperature in C:");
  Serial.println(t);
  Serial.println("Humidity in C:");
  Serial.println(h);
  delay(1000);

  //rainfall
  sensorValue2 = digitalRead(sensorPin);
  if (sensorValue2==LOW){
  Serial.print("rain is detected stop the motor.");
   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  //digitalWrite(enable2, HIGH);
  }
  else{
  Serial.print("rain not detected ");
  }
 
  Serial.println();
  delay(100);

  //LCD 
   // set cursor to first column, first row
  lcd.setCursor(2, 0);
  // print message
  lcd.print("Hum in %:"+String(h));
  lcd.setCursor(2,1);
  lcd.print("Mois in %: "+String(moisture_percentage));
  delay(1000);
  lcd.clear();

  delay(1000);

  //ThongSpeak
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(moisture_percentage);
    postStr += "&field2=";
    postStr += String(sensorValue2);
    postStr += "&field3=";
    postStr += String(h);
    postStr += "&field4=";
    postStr += String(distanceCm);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

}
}
