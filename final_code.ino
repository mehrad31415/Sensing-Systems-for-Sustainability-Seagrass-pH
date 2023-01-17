#include <SPI.h>
#include <SD.h>
File myFile;
#include "Sodaq_DS3231.h"
DateTime now;
int newHour = 0;
int oldHour = 0;



#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
 
float ph_act;
// for the OLED display
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 
// Required libraries are imported
#include <DallasTemperature.h>
#include <OneWire.h>
// The input pin to which the sensor module is connected is declared here
#define LKTemp2 4
const int LDR = A0;
int input_val = 0;

// Libraries are configured
OneWire oneWire(LKTemp2);
DallasTemperature sensors(&oneWire);


void save_temperature() {
myFile = SD.open("test2.txt", FILE_WRITE);
now = rtc.now();
myFile.print(now.hour());
myFile.print(":");
myFile.print(now.minute());
rtc.convertTemperature(); //convert current temperature into registers
myFile.print(",");
myFile.println(rtc.getTemperature()); //read registers and save temperature on deg C
myFile.print("Temperature: ");
myFile.println(sensors.getTempCByIndex(0));
myFile.print("pH Val: ");
myFile.println(ph_act);
myFile.print("LDR Value is: ");
myFile.println(input_val);
myFile.close();
}







void setup() {
  Wire.begin();
  rtc.begin();
// Open serial communications and wait for port to open:
Serial.begin(9600);
Serial.println("LK-Temp2 temperature measurement");
  // Sensor is initialized
  sensors.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE); 

while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
Serial.print("Initializing SD card...");
if (!SD.begin(10)) {
Serial.println("initialization failed!");
while (1);
}
Serial.println("initialization done.");

now = rtc.now();
oldHour = now.hour();


}


 
void loop() {


 input_val = analogRead(LDR);
  Serial.print("LDR Value is: ");
  Serial.println(input_val);
  delay(1000);
  // Temperature measurement is started...
  sensors.requestTemperatures();
  // ... and measured temperature is printed
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" C");
  delay(1000); // 1s pause until next measurement

 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
 
 Serial.print("pH Val: ");
 Serial.println(ph_act);
 Serial.print("");
 delay(1000);

now = rtc.now();
newHour = now.hour();
if (oldHour != newHour) {
save_temperature();
oldHour = newHour;
}
}


//void save_temperature() {
  // if the file opened okay, write to it:
 //if (myFile) {
//Serial.print("Writing to temp.txt...");
//myFile = SD.open("temp.txt", FILE_WRITE);
//myFile.print("Temperature: ");
//myFile.println(sensors.getTempCByIndex(0));
// close the file:
//myFile.close();
//Serial.println("done.");
//} else {
// if the file didn't open, print an error:
//Serial.println("error opening test.txt");
//}
//}

//void save_ph() {
  // if the file opened okay, write to it:
 // if (myFile) {
// Serial.print("Writing to ph.txt...");
//myFile = SD.open("ph.txt", FILE_WRITE);
//myFile.print("pH Val: ");
//myFile.println(ph_act);
// close the file:
//myFile.close();
//Serial.println("done.");
//} else {
// if the file didn't open, print an error:
//Serial.println("error opening test.txt");
//}
//}


//void save_light() {
  // if the file opened okay, write to it:
//  if (myFile) {
// Serial.print("Writing to light.txt...");
//myFile = SD.open("light.txt", FILE_WRITE);
//myFile.print("LDR Value is: ");
//myFile.println(input_val);
// close the file:
//myFile.close();
//Serial.println("done.");
//} else {
// if the file didn't open, print an error:
//Serial.println("error opening test.txt");
//}
//}
