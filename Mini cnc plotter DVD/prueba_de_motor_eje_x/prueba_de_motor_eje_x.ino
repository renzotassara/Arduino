#include <Stepper.h>
 
const int stepsPerRevolution = 20; 
 
// Connection pins:
Stepper myStepperY(stepsPerRevolution, 2,3,4,5);
 
void setup() {
 // Set speed:
 myStepperY.setSpeed(100);
 // max 250 steps for dvd/cd stepper motor
 myStepperY.step(160); 
 delay(100);
 
}
 
void loop() {
 
}
