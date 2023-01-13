#include <Arduino.h>
#include "Robot.h"
//Robot robot;
Wheel test1{
  -45,//Winkel zu Frame
  5,//x
  5,//y
  5//radius
};
Motor motor1(test1, 0);
Motor motor2(test1, 1);
void setup() {
  motor1.setSpeed(50);
  motor2.setSpeed(50);
  delay(3000);
  motor1.setSpeed(0);
  motor2.setSpeed(0);
}

void loop() {
  // put your main code here, to run repeatedly:
}

