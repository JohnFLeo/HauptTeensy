#include <Arduino.h>
#include "Robot.h"
#include "Kompass.h"
#include "Led.h"
Robot robot;
Kompass kompass;
Led led;
int zustand;
bool teamblue = false;bool attacker = false;bool running = false;bool i4 = false;
#define B_RO_PIN 39
#define B_RU_PIN 37
#define B_LO_PIN 38
#define B_LU_PIN 36

//-----declaration-----------
void initKompass();
void initButtons();
void testLed();
//---------------------------

void setup() {
  Serial.begin(9600);
  led.initLed();
  initButtons();
  initKompass();
  zustand = 0;
}
void loop(){
  switch (zustand){
  case 0:
    idle();
    break;
  case 1:
    driveToBall();
    break;
  case 2:
    driveToGoal();
    break;
  case 3:
    shoot();
    break;
  default:
    idle();
    break;
  }
}
//--------Zustands-Automat--------------------
void idle(){
  //Tue nichts
  if(running) zustand = 1;
}
void driveToBall(){
  if(!running) zustand = 0;
}
void driveToGoal(){
  if(!running) zustand = 0;
}
void shoot(){
  if(!running) zustand = 0;
}
//-----------Kompass-------------------
void initKompass(){
  if(!kompass.starten()){
    while (true){//kompass funkt nicht
      Serial.println("!!Fehler-bno!!");
      led.setLedColor(4, ROT);
      delay(1000);
    }
  }
  led.setLedColor(4, GRUEN);
}
//-----------Toggle-Switch-Logic----------------
bool teamblue = false;bool attacker = false;bool running = false;bool i4 = false;
void InterruptHandler1(){
  if(teamblue){teamblue = false;led.setLedColor(0,GELB);}else{teamblue = true; led.setLedColor(0,BLAU);}
}
void InterruptHandler2(){
  if(attacker){attacker = false;led.setLedColor(1,ROT);}else{attacker = true; led.setLedColor(1,GELB);}
}
void InterruptHandler3(){
  if(running){running = false;led.setLedColor(2,ROT);}else{running = true; led.setLedColor(2,WEISS);}
}
void InterruptHandler4(){
  if(i4){i4 = false;led.setLedColor(3,ROT);}else{i4 = true; led.setLedColor(3,BLAU);}
}
void initButtons(){
  attachInterrupt(digitalPinToInterrupt(B_RO_PIN), InterruptHandler1,RISING);//Team select
  attachInterrupt(digitalPinToInterrupt(B_RU_PIN), InterruptHandler2,RISING);//Attacking or Defending
  attachInterrupt(digitalPinToInterrupt(B_LO_PIN), InterruptHandler3,RISING);//Start&Stop
  attachInterrupt(digitalPinToInterrupt(B_LU_PIN), InterruptHandler4,RISING);//---------
}
//-----------Test--------------------------
void testLed(){
  for(int i = 0; i< 8; i++){
    led.fill(ROT);
    led.setLedColor(i,GRUEN);
    delay(1000);
  }
}
//-------------------------------------------