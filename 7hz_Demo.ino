#include <Arduino.h>
#include <Wire.h>           // This library is included with the Arduino IDE, and allows communication with I2C/TWI devices
#include "Sodaq_DS3231.h"   // Install this library to interact with the Real Time Clock

#define LOGGERNAME "MeOw Station [5]"
#define PROJECT_LOCATION "Project & Location: Spartina, Smith Island NC"
#define INSTALL_DATE "Installation Date: 20-22 August 2019"
#define DATA_HEADER "DateTime,UnixTime,BoardTemp_C,Battery_V,SonarRange_Bed_mm,SonarRange_Water_mm"
#define FILE_NAME "DataLog5.txt"  

/////////////////////////////////////////////////////////////////////////////////////////////////////////////  

#define LOGGERNAME "MeOw Station [5]"
#define PROJECT_LOCATION "Project & Location: Spartina, Smith Island NC"
#define INSTALL_DATE "Installation Date: 20-22 August 2019"
#define DATA_HEADER "DateTime,UnixTime,BoardTemp_C,Battery_V,SonarRange_Bed_mm,SonarRange_Water_mm"
#define FILE_NAME "DataLog5.txt" 

#include <Wire.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <SD.h>

#include <Arduino.h>
#include <SDL_Arduino_SSD1306.h>
#include <AMAdafruit_GFX.h>

#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt_PCINT0.h>

#include <SoftwareSerial_PCINT12.h>
const int SonarExcite_Bed = 6;
const int SonarExcite_Water = 7;
SoftwareSerial sonarSerial_Bed(10, -1);
SoftwareSerial sonarSerial_Water(11, -1);      
 
boolean stringComplete_Bed = false;
boolean stringComplete_Water = false;
 
#define READ_DELAY 1
 
//RTC Timer
RTCTimer timer;

String dataRec = "";
int currentminute;
int currentsecond;
long currentunixtime = 0;
float boardtemp = 0.0;

int batteryPin = A6;    //Select the input pin for the potentiometer
int batterysenseValue = 0;  //Variable to store the value coming from the sensor
float batteryvoltage;
float batterypercent;

int range_mm_Bed = 100 ;
int range_mm_Water = 100;

//RTC Interrupt pin
#define RTC_PIN A7
#define RTC_INT_PERIOD EveryMinute

#define SD_SS_PIN 12

//I2C pin
SDL_Arduino_SSD1306 display(4);

int State8 = LOW;
int State9 = LOW;

int LEDtime = 100;   //milliseconds

void setup ()

{

   //Initialize serial display
  Serial.begin(57600);
  
  //Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println();
  display.println(LOGGERNAME);
  display.display();
  
  //Initialise the sonar serial connection
  sonarSerial_Bed.begin(9600);
  sonarSerial_Water.begin(9600);
  rtc.begin();
  delay(100);

  //Bed
  pinMode(SonarExcite_Bed, OUTPUT);
  digitalWrite(SonarExcite_Bed, LOW);  //Power pin for the ultrasonic sensor

  //Water
  pinMode(SonarExcite_Water, OUTPUT);
  digitalWrite(SonarExcite_Water, LOW);  //Power pin for the ultrasonic sensor
  
  Serial.println("Power On, running");
  Serial.println("Date,Time,UnixTime,BoardTemp_C,Battery_V,SonarRange_Bed_mm,SonarRange_Water_mm");
  
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    Serial.begin(57600);
    Wire.begin();
    rtc.begin();

    Serial.println("EnviroDIY Mayfly: Blink demo with serial temperature");

}

void loop()

{

    if (State8 == LOW) {
        State8 = HIGH;
    } else {
        State8 = LOW;
    }

          ////////////////   Bed   ////////////////   
          digitalWrite(SonarExcite_Bed, HIGH);
          ///range_mm_Bed = SonarRead_Bed();
          digitalWrite(SonarExcite_Bed, LOW);
          stringComplete_Bed = false; 

          ////////////////  Water  ////////////////
          digitalWrite(SonarExcite_Water, HIGH);
          ///range_mm_Water = SonarRead_Water(); 
          digitalWrite(SonarExcite_Water, LOW); 
          stringComplete_Water = false; 
    
    digitalWrite(8, State8);

    State9 = !State8;
    digitalWrite(9, State9);

    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature(),2); //read registers and display the temperature
    Serial.println("deg C");

    delay(LEDtime);
    
}
