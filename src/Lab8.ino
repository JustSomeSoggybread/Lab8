#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"

VCNL4040 proximitySensor;

byte errorResult;           // error code returned by I2C Wire.endTransmission()
byte i2c_addr;              // I2C address being pinged
byte lowerAddress = 0x08;   // I2C lowest valid address in range
byte upperAddress = 0x77;   // I2C highest valid address in range
byte id = 0x10;

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(D3, OUTPUT); //blue
  pinMode(D5, OUTPUT); //green
  pinMode(D7, OUTPUT); //red

  pinMode(A1, INPUT); // blue button (C)
  pinMode(A3, INPUT); // green button (B)
  pinMode(A5, INPUT); // red button (A)

  Serial.begin(9600);
  proximitySensor.begin();
  proximitySensor.powerOnProximity();
  proximitySensor.powerOnAmbient();


//scrolls through addresses until the device responds, at which point set ID to that address
  for (i2c_addr = lowerAddress; i2c_addr <= upperAddress; i2c_addr++){
    Wire.beginTransmission(i2c_addr);             
    errorResult = Wire.endTransmission();                 
    if (errorResult == 0)                                
    {
        id = i2c_addr;
   }
  }  

  Wire.begin(); //Wire,begin() down here because finding I2C address ends transmission


}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  //proximity and light values
  unsigned int proximityVal = proximitySensor.getProximity();
  unsigned int lightVal = proximitySensor.getAmbient();

  // set lights for proximity - closer = higher proximity
  
  if (proximityVal < 40){
    light(HIGH, LOW, LOW);
  }
  else if (proximityVal < 150){
    light(LOW, HIGH, LOW);
  }

  else{
    light(LOW, LOW, HIGH);
  }

  //sets buttons
  //200 ms delay so serial monitor doesn't overload
  if (digitalRead(A5) == HIGH){
    Serial.print("Proximity: ");
    Serial.println(proximityVal);
    delay(200);
  }

  else if (digitalRead(A3) == HIGH){
    Serial.print("Light value: ");
    Serial.println(lightVal);
    delay(200);
  }

  else if (digitalRead(A1) == HIGH){
    Serial.print("I2C address: ");
    Serial.println(id);
    delay(200);
    
  }
}

//streamlining turning on the LEDSs
void light(int blue, int green, int red){
  digitalWrite(D3, blue);
  digitalWrite(D5, green);
  digitalWrite(D7, red);
}
