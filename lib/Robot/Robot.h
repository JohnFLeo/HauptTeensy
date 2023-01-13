#ifndef _ROBOT_H_
#define _ROBOT_H_
#include <Arduino.h>
#include "Motor_neu.h"
class Robot{
private:
  Wheel VL{
    -45,//Winkel zu Frame
    5,//x
    5,//y
    5//radius
  };
  Wheel VR{
    -135,
    5,
    -5,
    5
  };
  Wheel HL{
    45,
    -5,
    5,
    5
  };
  Wheel HR{
    135,
    -5,
    -5,
    5
  };
  Motor Motor_VL = Motor(VL,0);
  Motor Motor_VR = Motor(VR,1);
  Motor Motor_HL = Motor(HL,2);
  Motor Motor_HR = Motor(HR,3);

public:
  Robot();
  void drive(int velocity, int angle, int angularVelocity);
  int getMaxVelocity(int angle, int maxVelocity);
};
#endif
