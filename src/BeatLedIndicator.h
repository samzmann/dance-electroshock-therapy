#ifndef BEAT_LED_INDICATOR_H
#define BEAT_LED_INDICATOR_H

#include <Arduino.h>
#include <FastLED.h>
#include <Ramp.h>

#define NUM_LEDS 10

class BeatLedIndicator
{
public:
    void initialize();
    void pulseOn();
    void pulseOff();

private:
    CRGB leds[NUM_LEDS];
    int numLeds;
    ramp brightness;
};

#endif // BEAT_LED_INDICATOR_H