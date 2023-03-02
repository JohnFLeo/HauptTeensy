#include <Arduino.h>
#include "Robot.h"
#include "Kompass.h"
#include "Led.h"
Robot robot;
Kompass kompass;
Led led;
int zustand;
int zielWinkel;
int zielDistanz;
int drehung;
bool teamblue = false;bool attacker = false;bool running = false;bool i4 = false;
#define B_RO_PIN 39
#define B_RU_PIN 37
#define B_LO_PIN 38
#define B_LU_PIN 36

//-----declaration-----------
void initKompass();
void initButtons();
void testLed();
//--------Linie---------------------
void liniePruefen(){
  int length = Serial2.available();
  for(int i=0; i<length;i++) {
    int msg = Serial2.read();
    if(msg!=0){
      Serial.println(msg);
    }
  }
}
//--------PID-Fahren--------------------------
int errorDrehung;
int errorDistanz;
int errorWinkel;
void pidDrive(){
  errorDrehung = kompass.zWertAuslesen()*1;
  errorDistanz = zielDistanz*1;
  errorWinkel = zielWinkel*1;
  robot.drive(0, 0, errorDrehung);
}
//--------Zustands-Automat--------------------
void idle(){
  //Tue nichts
  robot.drive(0,0,0);
  if(running){
    Serial.println("Change to Running"); 
    zustand = 1;
    led.fill(WEISS);
    led.setLedColor(0,GRUEN);
  }
}
void driveToBall(){
  liniePruefen();
  //datenVonPiAnfordern(BALL);
  //pidDrive();
  if(!running){ 
    Serial.println("Change to Idle");
    zustand = 0;
    led.fill(WEISS);
    led.setLedColor(0,GELB);
  }
}
void driveToGoal(){
  if(!running){ 
    zustand = 0;
    led.fill(WEISS);
    led.setLedColor(0,GELB);
  }
}
void shoot(){
  if(!running){ 
    zustand = 0;
    led.fill(WEISS);
    led.setLedColor(0,GELB);
  }
}
//-------Pi-Com-------------------------------
typedef enum{
  TOR_GELB = 0,
  TOR_BLAU = 1,
  BALL = 2
}CamCom;
void datenVonPiAnfordern(CamCom index){
  switch (index){
  case TOR_BLAU:
    Serial1.write('b');
    delay(50);
    break;
  case TOR_GELB:
    Serial1.write('c');
    break;
  case BALL:
    Serial1.write('a');
    int length = Serial1.available();
    for(int i=0; i<length;i++) {
      int msg = Serial1.read();
      switch (i){
      case 0:
        zielDistanz = msg;
        break;
      case 1:
        zielWinkel = msg;//mit Paul besprechen
        break;
      case 2:
        zielWinkel = msg;
        break;
      default:
        break;
      }
    }
    break;
  default:
    break;
  }
}
//--------------------------------------------------
void setup() {
  Serial.begin(9600);
  //Serial1.begin(9600);
  Serial2.begin(9600);
  while(!Serial2){
    Serial.println("Kein Serial2");
  }
  led.initLed();
  initButtons();
  initKompass();
  zustand = 0;
  zielDistanz = 0;
  zielWinkel = 0;
  drehung = 0;
  led.setLedColor(0,GELB);
}
void loop(){
  switch (zustand){
  case 0:
    idle();
    break;
  case 1:
    driveToBall();
    break;
  default:
    break;
  }
}
//-----------Kompass-------------------
void initKompass(){
  if(!kompass.starten()){
    while (true){//kompass funkt nicht
      Serial.println("!!Fehler-bno!!");
      led.setLedColor(3, ROT);
      delay(1000);
    }
  }
  led.setLedColor(3, GRUEN);
}
//-----------Toggle-Switch-Logic----------------
void InterruptHandler1(){
  if(teamblue){teamblue = false;}else{teamblue = true; }
}
void InterruptHandler2(){
  if(attacker){attacker = false;}else{attacker = true; }
}
void InterruptHandler3(){
  if(running){running = false;}else{running = true;}
}
void InterruptHandler4(){
  if(i4){i4 = false;}else{i4 = true;}
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