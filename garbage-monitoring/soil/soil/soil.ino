const int AirValue = 2970;
const int WaterValue = 1200;
int soilmoisturepercent = 0;
#define moistureSensor A0
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(moistureSensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int baca = analogRead(moistureSensor);
  Serial.println(baca);
  Serial.print("Persenan: ");
  soilmoisturepercent = map(baca, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent > 100){
    Serial.println ("100%");  
  }
  else if(soilmoisturepercent < 0){
    Serial.println ("0%");  
  }
  else if(soilmoisturepercent >= 0 && soilmoisturepercent <=100){
    Serial.print (soilmoisturepercent);
    Serial.println ("%");  
  }
  delay(500);
}
