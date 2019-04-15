#include <AccelStepper.h>

AccelStepper stepper(1, 9, 8);
int pos = 100;

void setup() {
  stepper.setMaxSpeed(300);
  stepper.setAcceleration(1000);
}

void loop() {
  if (stepper.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);
  }
  stepper.run();

}
