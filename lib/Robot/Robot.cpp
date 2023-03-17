#include "Robot.h"
Robot::Robot(){
  Motor_VL.init();
  Motor_VR.init();
  Motor_HL.init();
  Motor_HR.init();
}

void Robot::drive(int velocity, int angle, int angularVelocity){
  int maxV = getMaxVelocity(angle, 255 + angularVelocity);
  //Serial.print("MaxV: ");Serial.println( maxV);
  if(velocity> maxV && velocity >= 0){
    Motor_VL.drive(angle, maxV, angularVelocity);
    Motor_VR.drive(angle, maxV, angularVelocity);
    Motor_HL.drive(angle, maxV, angularVelocity);
    Motor_HR.drive(angle, maxV, angularVelocity);
  }else if(velocity <-maxV && velocity < 0){
    Motor_VL.drive(angle, -maxV, angularVelocity);
    Motor_VR.drive(angle, -maxV, angularVelocity);
    Motor_HL.drive(angle, -maxV, angularVelocity);
    Motor_HR.drive(angle, -maxV, angularVelocity);
  }else{
    Motor_VL.drive(angle, velocity, angularVelocity);
    Motor_VR.drive(angle, velocity, angularVelocity);
    Motor_HL.drive(angle, velocity, angularVelocity);
    Motor_HR.drive(angle, velocity, angularVelocity);
  }
  //Serial.println();
}
void Robot::motorFahren(int index){
  switch (index)
  {
  case 1:
    Motor_VL.setSpeed(100);
    break;
  case 3:
    Motor_VR.setSpeed(100);
    break;
  case 0:
    Motor_HL.setSpeed(100);
    break;
  case 2:
    Motor_HR.setSpeed(100);
    break;
  default:
    break;
  }
}
int Robot::getMaxVelocity(int angle, int maxVelocity){
  int m1 = abs(Motor_VL.getMaxVelocity(angle, maxVelocity));
  int m2 = abs(Motor_VR.getMaxVelocity(angle, maxVelocity));
  int m3 = abs(Motor_HL.getMaxVelocity(angle, maxVelocity));
  int m4 = abs(Motor_HR.getMaxVelocity(angle, maxVelocity));
  /*
  Serial.print(Motor_VL.getMaxVelocity(angle, maxVelocity));Serial.print("\t\t");
  Serial.print(Motor_VR.getMaxVelocity(angle, maxVelocity));Serial.print("\t\t");
  Serial.print(Motor_HL.getMaxVelocity(angle, maxVelocity));Serial.print("\t\t");
  Serial.print(Motor_HR.getMaxVelocity(angle, maxVelocity));Serial.println("\t\t");*/
  return min(min(m1,m2),min(m3,m4));
}
