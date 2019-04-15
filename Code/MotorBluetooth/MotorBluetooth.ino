 // Include the Arduino Stepper Library
#include <AccelStepper.h>
#include <SoftwareSerial.h>

AccelStepper stepper(1, 9, 8);
SoftwareSerial BTSerial(10,11);
int pos = 100;
char c;

bool initTurn = true;


void setup() {
  // org snelheid = 300;
  stepper.setMaxSpeed(150);
  stepper.setAcceleration(1000);
  BTSerial.begin(38400);
  Serial.begin(38400);
}

void loop() {
  if(BTSerial.available() > 0){
    c = BTSerial.read();
    switch(c){
      case '1':
        if(initTurn){
          initTurn = !initTurn;
        }
        Serial.println("UP");
        stepperTurn(pos);
        break;
      case '2':
        if(!initTurn){
         Serial.println("Down");
         stepperTurn(-pos); 
        }
        break;
    }
  }
  stepper.run();
}

void stepperTurn(int pos){
  if(stepper.distanceToGo() == 0){
    Serial.println("moving");
    stepper.moveTo(pos);
  }
}

