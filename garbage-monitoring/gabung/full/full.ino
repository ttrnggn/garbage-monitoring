#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define DS18B20PIN 16
#define sensorPhPin 36
#define moistureSensor 34
// WiFi
const char *ssid = "IPMosa"; // Enter your WiFi name
const char *password = "PKLUB1Power";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *suhuTopic = "GEb5X2BJ/PKLIPsuhu";
const char *moistureTopic = "GEb5X2BJ/PKLIPmoisture";
const char *phTopic = "GEb5X2BJ/PKLIPph";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

const int AirValue = 2970; // threshold sensor moisture saat di udara
const int WaterValue = 1200; // threshold sensor moisture saat di air
int soilmoisturepercent = 0;

int phSensorValue = 0;
float phTanah = 0.0;
char buffersuhu [10];
char bufferlembab [10];
char bufferph [4];
char persen [] = " %";
char derajat []  = " C";
WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
OneWire oneWire(DS18B20PIN);
DallasTemperature sensor(&oneWire);

void setup() {
  Serial.begin(115200);
  sensor.begin();
  pinMode(moistureSensor, INPUT);
  pinMode(sensorPhPin, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
 // client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(5000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("ESP Ready");
  // publish and subscribe
  client.publish(suhuTopic, "ESP Ready");
  client.publish(moistureTopic, "ESP Ready");
  client.publish(phTopic, "ESP Ready");
  display.display();
  delay(5000);
  display.clearDisplay();
  
}

void loop() {
  //client.loop();
  // -----------------Sensor Temperature-------------------
  display.clearDisplay();
  sensor.requestTemperatures(); 
  float tempinC = sensor.getTempCByIndex(0);
  Serial.print("Temperature = ");
  Serial.print(tempinC);
  Serial.println("C");
  display.setCursor(0, 30);
  display.print("Temp: ");
  display.print(tempinC);
  display.println("C");
  dtostrf(tempinC, 3, 1, buffersuhu);
  strcat(buffersuhu, derajat);
  client.publish(suhuTopic, buffersuhu);
  // -----------------Sensor Moisture-------------------
  int baca = analogRead(moistureSensor);
  Serial.println(baca);
  Serial.print("Kelembaban: ");
  display.print("Kelembaban: ");
  soilmoisturepercent = map(baca, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent > 100){
    Serial.println ("100%");  
    display.println ("100%");
    client.publish(moistureTopic, "100%");
  }
  else if(soilmoisturepercent < 0){
    Serial.println ("0%");  
    display.println ("0%");
    client.publish(moistureTopic, "0%");
  }
  else if(soilmoisturepercent >= 0 && soilmoisturepercent <=100){
    Serial.print (soilmoisturepercent);
    Serial.println ("%");  
    display.print (soilmoisturepercent);
    display.println ("%");
    dtostrf(soilmoisturepercent, 4, 2, bufferlembab);
    strcat(bufferlembab, persen);
    client.publish(moistureTopic, bufferlembab);
  }
  // -----------------Sensor Ph Tanah-------------------
  phSensorValue = analogRead(sensorPhPin);
  phTanah = (-0.0139*phSensorValue) + 7.7851;
  Serial.print("sensor ADC= ");
  Serial.print(phSensorValue);
  Serial.print("  output Ph= ");
  Serial.println(phTanah);
  display.print("Ph: ");
  display.print(phTanah);
  dtostrf(phTanah, 3, 2, bufferph);
  client.publish(phTopic, bufferph);
  delay(5000);
  display.display();
}
