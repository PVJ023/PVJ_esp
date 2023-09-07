#include <ESP8266WiFi.h>
#include <DHT.h>

//Include the library files
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

#define BLYNK_AUTH_TOKEN "zYGLTigqPVJmmcmwyZ2mfyWJkUgU81eV"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Shreyas";
char pass[] = "12345678";//Enter your WIFI password

#define DHTPIN 0  // DHT sensor pin
#define DHTTYPE DHT11  // DHT sensor type
#define LDRPIN 2  // LDR sensor pin
#define RAINPIN A0  // Rainfall sensor pin
#define BUZZPIN 5
#define LEDPIN 16

DHT dht(DHTPIN, DHTTYPE);
int ldrValue = 0;
int rainValue = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  pinMode(BUZZPIN,OUTPUT);
  pinMode(LEDPIN,OUTPUT);
  pinMode(RAINPIN,INPUT);
  pinMode(LDRPIN,INPUT);
  //Initialize the Blynk library
  
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  ldrValue = analogRead(LDRPIN);
  rainValue = analogRead(RAINPIN);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, h);  //V5 is for Humidity
  Blynk.virtualWrite(V2, t);  //V6 is for Temperature
  Blynk.virtualWrite(V1, rainValue); 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t Humidity: ");
  Serial.print(h);
  Serial.print(" %\t LDR: ");
  Serial.print(ldrValue);
  Serial.print("\t Rain: ");
  if (rainValue < 700) {
    Serial.println("It's raining");
    Serial.println(rainValue);
    digitalWrite (BUZZPIN, HIGH);
  } else {
    Serial.println("It's not raining");
    Serial.println(rainValue);
    digitalWrite (BUZZPIN, LOW);
  }

  if (ldrValue < 100) {
    Serial.println("It's DAY time");
    digitalWrite (LEDPIN, LOW);
  } else {
    Serial.println("It's NIGHT time");
    digitalWrite (LEDPIN, HIGH);
  }
  //Run the Blynk library
  Blynk.run();
}
