#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20PIN 2

/* Create an instance of OneWire */
OneWire oneWire(DS18B20PIN);

DallasTemperature sensor(&oneWire);

void setup()
{
  Serial.begin(9600);
  /* Start the DS18B20 Sensor */
  sensor.begin();
}

void loop()
{
  sensor.requestTemperatures(); 
  float tempinC = sensor.getTempCByIndex(0);
  Serial.print("Temperature = ");
  Serial.print(tempinC, 1);
  Serial.println("ºC");
}
