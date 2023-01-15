#include "Motor_neu.h"
//public
Motor::Motor(Wheel wheel,int nr){
  this->nr = nr;
  this->wheel = wheel;
}

void Motor::init() {
  pinMode(pwmPin[nr], OUTPUT);
  pinMode(dir1Pin[nr], OUTPUT);
  pinMode(dir2Pin[nr], OUTPUT);
}

void Motor::drive(int angle, int velocity, int angularVelocity){
  double angle2 = degreeToRadians(angle);
  double beta = degreeToRadians(wheel.angleToFrame);

  Coords cords = toCartesianVelocity(angle2, velocity);
  double vx = cords.x;
  double vy = cords.y;

  int v = //(1/wheel.radius) *
  (
    cos(beta) * vx+//(-wheel.yPos * angularVelocity + vx) +
    sin(beta) * vy//( wheel.xPos * angularVelocity + vy)
  )+angularVelocity;
  //Serial.print("M_");Serial.print(nr);Serial.print(": ");Serial.print(v);Serial.print("   ");
  setSpeed(v);
}

void Motor::setSpeed(int velocity){
  if(velocity < 0 && velocity >= -255){
    //reverse
    velocity = -velocity;
    //dir1 = 1
    digitalWrite(dir1Pin[nr], 1);
    //dir2 = 0
    digitalWrite(dir2Pin[nr], 0);
    analogWrite(pwmPin[nr], velocity);
  }else if(velocity > 0 && velocity <= 255){
    //forward
    //dir1 = 0
    digitalWrite(dir1Pin[nr], 0);
    //dir2 = 1
    digitalWrite(dir2Pin[nr], 1);
    analogWrite(pwmPin[nr], velocity);
  }else if(velocity == 0){
    //stop
    //dir1 = 0
    digitalWrite(dir1Pin[nr], 0);
    //dir2 = 0
    digitalWrite(dir2Pin[nr], 0);
    analogWrite(pwmPin[nr], velocity);
  }
}

double Motor::getMaxVelocity(int angle, int maxWheelVelocity){
  //angle must have the same "origin" as wheel.angleToFrame
  double angle2 = degreeToRadians(angle);
  double beta = degreeToRadians(wheel.angleToFrame);
  double zwerg = cos(abs(beta-angle2));
  if(zwerg < 0.01 && zwerg>-0.01){
    return 2000;//sehr großer wert da rad nur rutscht
  }
  return maxWheelVelocity/zwerg;
}
//private
double Motor::degreeToRadians(int angle){
  return (angle* 3.14)/180;
}
Coords Motor::toCartesianVelocity(double angle, int velocity){
  Coords erg;
  erg.x = cos(angle)*velocity;
  erg.y = sin(angle)*velocity;
  return erg;
}
