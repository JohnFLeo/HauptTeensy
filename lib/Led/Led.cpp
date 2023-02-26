#include "Led.h"
void Led::initLed(){
  ledstrip.begin();
  ledstrip.setBrightness(100);
  ledstrip.fill(Adafruit_NeoPixel().Color(255,255,255));
  ledstrip.show();
}
void Led::setLedColor(int index, int color){
  if(color == BLAU){
    ledstrip.setPixelColor(index,0,0,255);//blau
  }else if(color == GELB){
    ledstrip.setPixelColor(index,255,255,0);//gelb
  }else if(color == ROT){
    ledstrip.setPixelColor(index,255,0,0);//rot
  }else if(color == GRUEN){
    ledstrip.setPixelColor(index,0,255,0);//grün
  }else if(color == WEISS){
    ledstrip.setPixelColor(index,255,255,255);//weiß
  }
  ledstrip.show();
}
void Led::fill(int color){
  if(color == BLAU){
    ledstrip.fill(Adafruit_NeoPixel().Color(0,0,255));
  }else if(color == GELB){
    ledstrip.fill(Adafruit_NeoPixel().Color(255,255,0));
  }else if(color == ROT){
    ledstrip.fill(Adafruit_NeoPixel().Color(255,0,0));
  }else if(color == GRUEN){
    ledstrip.fill(Adafruit_NeoPixel().Color(0,255,0));
  }else if(color == WEISS){
    ledstrip.fill(Adafruit_NeoPixel().Color(255,255,255));
  }
  ledstrip.show();
}