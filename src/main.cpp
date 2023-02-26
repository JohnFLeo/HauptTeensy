#include <Arduino.h>
#include "Robot.h"
#include "Kompass.h"
//18, 19 kompass
/*
Serial zu Pi:
  0->14,
  1->15 
ich schrebe zu pi:
  #b#    -> ball 
  #g_y#  -> tor gelb 
  #g_b#  -> tor blau
ich bekomme:
  #winkel_distanz#
*/
#define PIN_7 7
#define PIN_LED 6
#define PIN_5 5
#define PIN_4 4

Robot robot;
Kompass kompass;
void setup() {
  Serial.begin(9600);
  pinMode(PIN_7, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_5, OUTPUT);
  pinMode(PIN_4, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  if(!kompass.starten()){
    digitalWrite(PIN_LED, HIGH);
    while (true){
      Serial.println("!!Fehler-bno!!");
      delay(1000);
    }
  }
  digitalWrite(PIN_LED, LOW);
  //robot.drive(100,45,0);
}
void loop() {
  Serial.println(kompass.zWertAuslesen());
}
