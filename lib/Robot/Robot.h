#ifndef _ROBOT_H_
#define _ROBOT_H_
#include <Arduino.h>
#include "Motor_neu.h"
class Robot{
private:
  Wheel VL{
    145,//Winkel zu Frame //-45
    5,//x
    5,//y
    5//radius
  };
  Wheel VR{
    35,//-135
    5,
    -5,
    5
  };
  Wheel HL{
    -145,//45
    -5,
    5,
    5
  };
  Wheel HR{
    -35,//135
    -5,
    -5,
    5
  };
  Motor Motor_VL = Motor(VL,1);//1
  Motor Motor_VR = Motor(VR,3);//3
  Motor Motor_HL = Motor(HL,0);//0
  Motor Motor_HR = Motor(HR,2);//2

public:
  Robot();
  void drive(int velocity, int angle, int angularVelocity);
  int getMaxVelocity(int angle, int maxVelocity);
  void motorFahren(int index);
};
#endif
