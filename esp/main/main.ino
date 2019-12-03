//Libs
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//Constants
#define DHTTYPE DHT11
#define SENSOR_PIN 5

//For Temp. Sensing
DHT dht(SENSOR_PIN, DHTTYPE);

//Localhost PHPMyadmin Details
IPAddress server_addr(192, 168, 0, 103);
char user[] = "niknodemcu";
char password[] = "password";

//WiFi Credentials
char ssid[] = "Nik";
char pass[] = "pramukhmahant31";

//WiFi Client Setup for Localhost
WiFiClient client;
WiFiServer server(80);
//MYSQL Connection Object
MySQL_Connection conn((Client *)&client);

bool connSt;
const int noisePin = D1;
const int gasPin = D2;
const int flamePin = D3;

//Query
char INSERT_SQL_DB[] = "INSERT INTO node_data_db.sensor_info (temp_stat, gas_stat, noise_stat, flame_stat) VALUES ('%s', '%s')";
char mainQuery[128];

void setup() {
  pinMode(noisePin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(flamePin, INPUT);
  Serial.begin(9600);
  delay(500);
  connectWIFI();
  connectDB();
}

void loop() {
  float temperatureVal = dht.readTemperature();
  int noiseVal = digitalRead(noisePin);
  int gasVal = digitalRead(gasPin);
  int flameVal = digitalRead(flamePin);

  if (connSt == false) {
    connectDB();
  }

  String temp, gas, noise, flame = "";
  
  /*//Converting all the values to string
  String temp = String(temperatureVal, 2);
  String gas = String(gasVal);
  String noise = String(noiseVal);
  String flame = String(flameVal);*/

  if (temperatureVal > 40){
    temp = "Temprature critical level";
  }else{
    temp = "Temprature normal level";
  }

  if (gasVal == HIGH){
    gas = "Harmful gas detected";
  }else{
    gas = "Gas status normal";
  }

  if (noiseVal == HIGH){
    noise = "Noise higher than normal";
  }else{
    noise = "Noise status normal";
  }

  if (flameVal == HIGH){
    flame = "Flame detected";
  }else{
    flame = "Environmemt clear";
  }

  //Converting string values to char array
  char cTemp[temp.length()];
  char cGas[gas.length()];
  char cNoise[noise.length()];
  char cFlame[flame.length()];
  
  temp.toCharArray(cTemp, temp.length());
  gas.toCharArray(cGas, gas.length());
  noise.toCharArray(cSound, noise.length());
  flame.toCharArray(cFlame, flame.length());

  //MYSQL database entry
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  sprintf(mainQuery, INSERT_SQL_DB, cTemp, cGas, cNoise, cFlame);
  cur_mem->execute(mainQuery);
  delete cur_mem;
  closeDB();
  delay(10000);
}

void connectDB() {
  Serial.println("Connecting to database...");
  if (conn.connect(server_addr, 3306, user, password)) {
    connSt = true;
    Serial.println("DB Connection Created");
  } else {
    Serial.println("A Connection Could not be Established with Database, Trying Again");
    connSt = false;
  }
}

void closeDB() {
  conn.close();
  connSt = false;
  Serial.println("DB Connection Closed");
}

void connectWIFI() {
  WiFi.begin(ssid, pass);
  int count = 0;
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    if (count > 60) {
      count = 0;
      WiFi.disconnect();
      ESP.eraseConfig();
      ESP.restart();
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP());
  WiFi.mode(WIFI_STA);
}

