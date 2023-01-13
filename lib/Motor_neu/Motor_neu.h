#ifndef _MOTOR_NEU_H_
#define _MOTOR_NEU_H_
#include <Arduino.h>
//usb zu mir gerichtet 
//M1: HR
//M2: VR
//speed pins
#define M1_PWM 2 
#define M2_PWM 29
#define M3_PWM -1
#define M4_PWM -1
//direction pin 1
#define M1_IN1 6
#define M2_IN1 27
#define M3_IN1 -1
#define M4_IN1 -1
//direction pin 2
#define M1_IN2 7
#define M2_IN2 26
#define M3_IN2 -1
#define M4_IN2 -1

struct Wheel{
   int angleToFrame;
   int xPos;
   int yPos;
   int radius;
};
struct Coords{
  double x;
  double y;
};
class Motor{
private:
  int nr;
  Wheel wheel;
  Coords toCartesianVelocity(double angle, int velocity);
  double degreeToRadians(int angle);
  uint8_t pwmPin[4] ={
    M1_PWM,
    M2_PWM,
    M3_PWM,
    M4_PWM
  };
  uint8_t dir1Pin[4] ={
    M1_IN1,
    M2_IN1,
    M3_IN1,
    M4_IN1
  };
  uint8_t dir2Pin[4] ={
    M1_IN2,
    M2_IN2,
    M3_IN2,
    M4_IN2
  };
  
public:
  Motor(Wheel, int nr);
  void init();
  void drive(int angle, int velocity, int angularVelocity);
  void setSpeed(int velocity);
  double getMaxVelocity(int angle, int maxWheelVelocity);
};
#endif
