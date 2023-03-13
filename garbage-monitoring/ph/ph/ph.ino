//tes sensor pH tanah 

/*
wiring:
kabel putih -> GND
kabel hitam -> output to A0
*/

//analog pin 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define analogInPin 36  //sambungkan kabel hitam (output) ke pin A0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//variable
int sensorValue = 0;        //ADC value from sensor
float outputValue = 0.0;        //pH value after conversion

void setup() {
  //initialize serial communications at 9600 bps:
  pinMode(analogInPin, INPUT);
  Serial.begin(115200);
  // if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;);
  // }
 // delay(2000);
  // display.clearDisplay();

  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // display.setCursor(0, 10);
  // display.print("Initiating");
  // delay(1000);
  // display.print(".");
  // delay(1000);
  // display.print(".");
  // delay(1000);
  // display.print(".");
  // delay(1000);
  // display.display();
  // delay(5000);
  // display.clearDisplay();
}

void loop() {
  //read the analog in value:
 // display.clearDisplay();
  sensorValue = analogRead(analogInPin);

  //Mathematical conversion from ADC to pH
  //rumus didapat berdasarkan datasheet 
  outputValue = (-0.0139*sensorValue)+7.7851;

  //print the results to the serial monitor:
  Serial.print("sensor ADC= ");
  Serial.print(sensorValue);
  Serial.print("  output Ph= ");
  Serial.println(outputValue);
  // display.setCursor(0, 30);
  // display.print("Ph: ");
  // display.print(outputValue);
  delay(1000);
 // display.display();
}
