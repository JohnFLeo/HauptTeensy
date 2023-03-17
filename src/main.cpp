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
void Bodeninterrupt();
void initKompass();
void pidDrive(int winkel, int v);
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
  Serial.begin(9600);//Computer usb
  Serial2.begin(9600);//bodenteensy
  while(!Serial2){
    Serial.println("Kein BodenTeensy");
  }
  Serial1.begin(9600);//Raspi
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
  case 2:
    Bodeninterrupt();
    break;
  default:
    Serial.println("!!overflow zustand!!");
    break;
  }
}
bool linie= false;
//--------Zustands-Automat--------------------
void Bodeninterrupt(){
  pidDrive(ziel_winkel+180 ,150);
  liniePruefen();
  if(!linie){
    zustand = 1;
  }
}
void idle(){
  //Tue nichts
  //datenVonPiAnfordern(BALL);
  //Serial.println(ziel_winkel);
  Serial2.begin(9600);
  pidDrive(0,0);
  //robot.motorFahren(1);
  //robot.drive(0,0,255);
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
    datenVonPiAnfordern(BALL);
    //ziel_winkel = 90; 
    Serial.println(ziel_winkel);
    pidDrive(ziel_winkel,200);
    //robot.drive(200,0,0);
  }
}//--------PID-Fahren--------------------------
int errorDrehung;int errorDrehung_last = 0;int errorDrehung_integral = 0;
void pidDrive( int winkel,int v){
  robot.drive(v, winkel+1*kompass.zWertAuslesen(), 1*kompass.zWertAuslesen());
}
//--------Linie---------------------
void liniePruefen(){
  int length = Serial2.available();
  for(int i=0; i<length;i++) {
    int msgT = Serial2.read();
    linie = false;
    //Serial.println(msgT);
    if(msgT!=0){
      pidDrive(180,10000);
      linie = true;zustand = 2;
      led.setLedColor(2, GELB); 
      Serial.println(msgT);
    }
  }
}
//-------Pi-Com-------------------------------
void read3Bytes(){
  if(Serial1.available()>0){
    Serial1.readBytes(msg,3);
  }
  if(msg[1] == 0b00000000){//wenn vorzeichen byte == 0b00000001 -> negativer winkel
    ziel_winkel = ((int)msg[2]);
  }else if(msg[1]== 0b00000001){
    ziel_winkel = (-(int)msg[2]);
  }else{
    Serial.println("Fehler!");
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
    Serial.println("a");
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
  led.setLedColor(3, BLAU);
  //kompass.resetZwert();
  //delay(100);
  initKompass();
  //delay(1000);
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