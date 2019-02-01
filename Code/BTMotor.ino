 // Include the Arduino Stepper Library
#include <Stepper.h>
#include <SoftwareSerial.h>
// Define Constants

// Number of steps per output rotation
const int STEPS_PER_REV = 200;
char c;
Stepper stepper_NEMA17(STEPS_PER_REV, 4, 5, 6, 7);
SoftwareSerial BTSerial(10,11);

void setup() {
  stepper_NEMA17.setSpeed(60);
  BTSerial.begin(38400);
  Serial.begin(38400);
}

void loop() {
  if(BTSerial.available() > 0){
    c = BTSerial.read();
    

    switch(c){
      case '1':
        stepper_NEMA17.step(120);
        Serial.println("UP");
        break;
      case '2':
        stepper_NEMA17.step(-120);
        Serial.println("Down");
        break;
    }
  }
}
