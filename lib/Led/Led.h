#ifndef _LED_H_
#define _LED_H_
#include <Adafruit_NeoPixel.h>

#define LED_PIN 13
enum{
  BLAU = 0,
  GELB = 1,
  ROT = 2,
  GRUEN = 3,
  WEISS = 4 
};
class Led{
private:
    Adafruit_NeoPixel ledstrip = Adafruit_NeoPixel(8, LED_PIN);
public:
    void initLed();
    void setLedColor(int index, int color);
    void fill(int color);
};
#endif