#include <Arduino.h>
#include "Robot.h"
#include "Kompass.h"
#include "Led.h"

#define B_RO_PIN 39
#define B_RU_PIN 37
#define B_LO_PIN 38
#define B_LU_PIN 36

#define minimaldistanz 0
//----Objekte-------
Robot robot;
Kompass kompass;
Led led;
//-----Variablen------------
int zustand = 0;
//ist wert
int ziel_distanz = 0;
int ziel_winkel = 0;
//soll wert
int roboter_winkel = 180;

bool teamblue = false;
bool attacker = false;
bool running = false;
bool i4 = false;

typedef enum{
  TOR_GELB = 0,
  TOR_BLAU = 1,
  BALL = 2
}CamCom;

byte msg[3];
//-----declaration-----------
void initKompass();
void pidDrive(int winkel);
void datenVonPiAnfordern(CamCom index);
void idle();
void driveToBall();
void driveToGoal();
void shoot();
void read3Bytes();
void liniePruefen();
void initButtons();
void testLed();
//---------Hauptmethoden-------------
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  while(!Serial2){
    Serial.println("Kein BodenTeensy");
  }
  Serial1.begin(9600);
  while (!Serial1){
    Serial.println("Kein Pi");
  }
  led.initLed();
  initButtons();
  initKompass();
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
    Serial.println("!!overflow zustand!!");
    break;
  }
}
bool linie= false;
//--------Zustands-Automat--------------------
void idle(){
  //Tue nichts
  if(linie){
    linie = false;
    pidDrive(180);
    delay(300);
  }
  robot.drive(0,0,0);
  Serial2.begin(9600);
  robot.drive(0,0,0);
  if(running){
    Serial.println("Change to Running"); 
    zustand = 1;
    led.setLedColor(0,GRUEN);
  }
}
void driveToBall(){
  if(!running){ 
    Serial.println("Change to Idle");
    zustand = 0;
    led.setLedColor(0,GELB);
  }else{
    liniePruefen();
    pidDrive(0);

  }
}//--------PID-Fahren--------------------------
int errorDrehung;int errorDrehung_last = 0;int errorDrehung_integral = 0;
void pidDrive( int winkel){
  robot.drive(10000, winkel+2*kompass.zWertAuslesen(), 2*kompass.zWertAuslesen());
}
//--------Linie---------------------
void liniePruefen(){
  int length = Serial2.available();
  for(int i=0; i<length;i++) {
    int msgT = Serial2.read();
    //Serial.println(msgT);
    if(msgT!=0){
      running = false;linie = true;
      led.setLedColor(2, GELB); 
      Serial.println(msgT);
    }
  }
}
//-------Pi-Com-------------------------------
void read3Bytes(){
  delay(15);
  if(Serial1.available()>0){
    Serial1.readBytes(msg,3);
  }
  ziel_distanz = (int)msg[0];
  if(msg[1] == 0b00000000){//wenn vorzeichen byte == 0b00000001 -> negativer winkel
    ziel_winkel = (-(int)msg[2])+90;
  }else{
    ziel_winkel = ((int)msg[2])+90;
  }
  
}
void datenVonPiAnfordern(CamCom index){
  switch (index){
  case TOR_BLAU:
    Serial1.println("b");read3Bytes();
    break;
  case TOR_GELB:
    Serial1.println("c");read3Bytes();
    break;
  case BALL:
    Serial1.println("a");read3Bytes();
    break;
  }
}
//-----------Kompass-------------------
void initKompass(){
  if(!kompass.starten()){
      Serial.println("!!Fehler-bno!!");
      led.setLedColor(3, ROT);
  }else{
    led.setLedColor(3, GRUEN);
  }
}
//-----------Toggle-Switch-Logic----------------
uint32_t timestamp1;uint32_t timestamp2;uint32_t timestamp3;uint32_t timestamp4;
void InterruptHandler1(){
  if((millis()-timestamp1 )<200)return;
  timestamp1 = millis();
  if(teamblue){teamblue = false;}else{teamblue = true; }
}
void InterruptHandler2(){
  if((millis()-timestamp2 )<200)return;
  timestamp2 = millis();
  if(attacker){attacker = false;}else{attacker = true; }
}
void InterruptHandler3(){
  if((millis()-timestamp3 )<200)return;
  timestamp3 = millis();
  if(running){running = false;}else{running = true;}
}
void InterruptHandler4(){
  if((millis()-timestamp4 )<200)return;
  timestamp4 = millis();
  led.setLedColor(3,BLAU);
  delay(100);
  initKompass();
  //Serial1.begin(9600);
  //Serial2.begin(9600);
  delay(500);
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