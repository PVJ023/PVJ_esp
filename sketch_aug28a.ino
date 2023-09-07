#include <FS.h>
#include<SPIFFS.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
/*space
  probe_id
  probe_description
  functional_uid
  dms_uid
  start_time
  end_time
  auto_reset
  probe_state
  probe_level
  probe_mode
  probe_off_delay
  sensor_logic
  sensor_id
  sensor_flag
  sensor_uid
  symbol_low
  limit_low
  symbol_high
  limit_high
  unit


*/

const char *ssid     = "Vedhyatech";
const char *password = "Vedhya_2608";

const long utcOffsetInSeconds =-19800;

const char* mqttServer = "192.168.29.64";
const int mqttPort = 1883;
const char* mqttUser = "PVJ";
const char* mqttPassword = "PVJ";

// Create an instance of the WiFiClient
WiFiClient wifiClient;
// Create an instance of the PubSubClient
PubSubClient client(wifiClient);


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void activateTemp() {
  int d = 0;
  int initialTemp = 20;
  while (d < 10) {
    delay(6000);
    initialTemp = initialTemp + 1;
    Serial.print("Temperature is : ");
    Serial.println(initialTemp);
    String str = String(initialTemp);
    char s[5];
    str.toCharArray(s, 5);
    client.publish("TempControl", s);
    d = d + 1;
  }
  if (d >= 10) {
    client.publish("TempFlag", "ProcessDone");
  }
}

void activateDust() {
  int d = 0;
  int dustLevel = 20;
  while (d < 10) {
    delay(6000);
    dustLevel = dustLevel + 1;
    Serial.print("Dust Level is : ");
    Serial.println(dustLevel);
    String str = String(dustLevel);
    char s[5];
    str.toCharArray(s, 5);
    client.publish("DustControl", s);
    d = d + 1;
  }
  if (d >= 10) {
    client.publish("DustFlag", "ProcessDone");
  }
}

void activateAirQ() {
  int d = 0;
  int AirQ = 20;
  while (d < 10) {
    delay(6000);
    AirQ = AirQ + 1;
    Serial.print("Temperature is : ");
    Serial.println(AirQ);
    String str = String(AirQ);
    char s[5];
    str.toCharArray(s, 5);
    client.publish("AirQControl", s);
    d = d + 1;
  }
  if (d >= 10) {
    client.publish("AirQFlag", "ProcessDone");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message received on topic:");
  Serial.println(topic);
  Serial.println("Message content:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set MQTT server and callback function
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Connect to MQTT broker
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32-Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  timeClient.begin();
}

void loop() {
  File file = SPIFFS.open("/standard_wfconfig.csv", "r"); // Open file for reading
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  timeClient.update();
  String time_current = String(timeClient.getFormattedTime()).substring(0,4) ;
  int sen_log;
  int s_time;
  int e_time;
  String header = file.readStringUntil('\n');
  int index = 0;
  String values[21];
  while (header.length() > 0) {
    int commaIndex = header.indexOf(",");
    if (commaIndex == -1) {
      values[index++] = header;
      break;
    }
    values[index++] = header.substring(0, commaIndex);
    header = header.substring(commaIndex + 1);
  }

  for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    if ( values[i] == "sensor_logic" ) {
      sen_log = i;
    }
    if ( values[i] == "start_time" ) {
      s_time = i;
    }
    if ( values[i] == "end_time" ) {
      e_time = i;
    }
  }

  int rownum = 0;
  int c = 0;
  int targetColumnIndex = s_time; // Index of the column which has data for time (0-based)
  Serial.println("Starting to print" );
  while (file.available()) {
    c++;
    String line = file.readStringUntil('\n'); // Read one line
    String value = getValue(line, ',', targetColumnIndex);
    value = value.substring(0, value.length() - 4);
    Serial.print(time_current);
    Serial.print(" ");
    Serial.println(value);
    if ( time_current == value ) {
      activateTemp();
      activateDust();
      activateAirQ();
    }
  }

  file.close();
  delay(10000); // Delay before reading again
}
