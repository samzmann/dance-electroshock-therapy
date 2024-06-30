#include "BeatLedIndicator.h"

void BeatLedIndicator::initialize()
{
    FastLED.addLeds<NEOPIXEL, 28>(leds, numLeds);
    FastLED.setBrightness(255);
    FastLED.clearData();

    // Serial.print("brightness: ");
    // Serial.println(brightness.getValue());
}

void BeatLedIndicator::pulseOn()
{
    fill_solid(leds, numLeds, CRGB::Red);
    FastLED.show();
}

void BeatLedIndicator::pulseOff()
{
    fill_solid(leds, numLeds, CRGB::Black);
    FastLED.show();
}