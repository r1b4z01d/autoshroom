// Libraries
#include <Adafruit_BME280.h>
#include "Adafruit_CCS811.h"

// Pins
int LEDPin = 13;
int FANPin = 1;
int FoggerPin = 12;

// Vars
double c02, temp, humidity;
bool fan, fogger;
unsigned long delayTime;

// Objects
Adafruit_BME280 bme; // I2C
Adafruit_CCS811 ccs; // I2C

void setup() {
    Serial.begin(9600);
    pinMode(LEDPin, OUTPUT);
    pinMode(FANPin, OUTPUT);
    pinMode(FoggerPin, OUTPUT);

    unsigned status;
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    }
    if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
  }
    delayTime = 1000;
}


void checkValues() { 

  if(humidity<80 && !fan){
    fogger = 1;
    digitalWrite(FoggerPin, HIGH);   //Turn on fogger
  }else{
    digitalWrite(FoggerPin, LOW);   //Turn off fogger
    fogger = 0;
  }
  
  if(temp>26.6 || c02 > 800){
    fogger = 0;
    digitalWrite(FoggerPin, LOW);   //Make sure fogger is off
    fan = 1;
    digitalWrite(FANPin, HIGH);   //Turn on fan
  }else{
    digitalWrite(FANPin, LOW);   //Turn off fan
    fan = 0;
  }

  if(fan||fogger){
    digitalWrite(LEDPin, HIGH);   //Turn on LED
  }else{
    digitalWrite(LEDPin, LOW);   //Turn of LED
  }
}

void printValues() { 
 Serial.print(c02);
 Serial.print(",");
 Serial.print(temp);
 Serial.print(",");
 Serial.print(humidity);
 Serial.print(",");
 Serial.print(fan);
 Serial.print(",");
 Serial.println(fogger);
}

void loop() { 
    updateValues();
    printValues();
    checkValues();
    delay(delayTime);
}

void updateValues() {
  temp = bme.readTemperature();
  humidity = bme.readHumidity();
  if (ccs.available()) {
    if(!ccs.readData()){
      c02 = ccs.geteCO2();
  
    }
    return true;
  }
  return false;
}
