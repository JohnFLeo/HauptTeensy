#include <Arduino.h>
#include "Robot.h"
#include "Kompass.h"
//18, 19 kompass
//
Robot robot;
Kompass kompass;
void setup() {
  kompass.starten();
  //robot.drive(100,45,0);
}
void loop() {
  
}
