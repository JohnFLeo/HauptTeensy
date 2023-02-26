#include <Arduino.h>
#include "Robot.h"
#include "Kompass.h"
#include "Led.h"
Robot robot;
Kompass kompass;
Led led;

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
  //robot.drive(100,45,0);
}
void loop(){
  Serial.println(kompass.zWertAuslesen());
}

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
bool i1 = false;bool i2 = false;bool i3 = false;bool i4 = false;
void InterruptHandler1(){
  if(i1){i1 = false;led.setLedColor(0,GELB);}else{i1 = true; led.setLedColor(0,BLAU);}
}
void InterruptHandler2(){
  if(i2){i2 = false;led.setLedColor(1,ROT);}else{i2 = true; led.setLedColor(1,GELB);}
}
void InterruptHandler3(){
  if(i3){i3 = false;led.setLedColor(2,ROT);}else{i3 = true; led.setLedColor(2,WEISS);}
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