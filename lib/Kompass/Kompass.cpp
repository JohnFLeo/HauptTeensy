#include "Kompass.h"
//Konstruktor
Kompass::Kompass(){
    this->bno = Adafruit_BNO055(55, 0x28);
}
//private
int Kompass::gibZWert(){
    sensors_event_t event;
    bno.getEvent(&event);
    return (int)event.orientation.x;
    delay(50);
}
//public
bool Kompass::starten(){
    if(!bno.begin()){   
        //Fehler!
        return false;
    }
    delay(500);
    bno.setExtCrystalUse(true);
    delay(500);
    zWertNull = gibZWert();
    return true;
}
int Kompass::zWertAuslesen(){
    int ausgelesenerZWert = gibZWert() - zWertNull;
    int ausgabe = 0;
    if( (ausgelesenerZWert) < 180 ){
        ausgabe = ausgelesenerZWert;
    }else {
        ausgabe = ausgelesenerZWert - 360;
    }
    return ausgabe;
}