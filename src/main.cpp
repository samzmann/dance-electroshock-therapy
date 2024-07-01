// FFT Test
//
// Compute a 1024 point Fast Fourier Transform (spectrum analysis)
// on audio connected to the Left Line-In pin.  By changing code,
// a synthetic sine wave can be input instead.
//
// The first 40 (of 512) frequency analysis bins are printed to
// the Arduino Serial Monitor.  Viewing the raw data can help you
// understand how the FFT works and what results to expect when
// using the data to control LEDs, motors, or other fun things!
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <FastLED.h>
#include <Ramp.h>

#include "BeatDetector.h"
#include "BpmCalculator.h"
#include "Position.h"
#include "ServoControl.h"

// Constants and Macros
#define NUM_LIMBS 4
#define NUM_LEDS 10
#define BPM 120
const int BPM_INTERVAL_MS = 60000 / BPM;

#define LED_FADE_IN_MS 20

// Audio components
AudioInputI2S audioInput;
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT256 myFFT;
// AudioAnalyzeFFT1024 myFFT;
AudioOutputI2S audioOutput;
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
AudioControlSGTL5000 audioShield;

// Beat detection and BPM calculation
BeatDetector beatDetector(myFFT);
BpmCalculator bpmCalculator(8);

// Servo controls
ServoControl servoControls[NUM_LIMBS] = {
    ServoControl(24, 90, 0),
    ServoControl(25, 90, 0),
    ServoControl(26, 90, 0),
    ServoControl(27, 90, 0),
};

// LED control
CRGB leds[NUM_LEDS];
rampByte ledBrightness;

// Stick figures for display
const char *stickFigures[] = {
    " o\n/|\\\n/ \\",
    " o/\n/|\n/ \\",
    "\\o\n |\\\n/ \\",
    "\\o/\n |\n/ \\",
    " o\n/|\\\n/ >",
    " o\n/|\\\n< \\",
    " o\n/|\\\n< >",
    " o/\n/|\n/ >",
    "\\o\n |\\\n< \\",
    "\\o\n |\\\n/ >",
    " o/\n/|\n< \\",
    "\\o/\n |\n/ >",
    "\\o/\n |\n< \\",
    " o/\n/|\n< >",
    "\\o\n |\\\n< >",
    "\\o/\n |\n< >"};

// Timing and state variables
int_least32_t lastBeatTimestamp = millis();
bool isLedOn = false;

void setup()
{
  // Initialize audio memory
  AudioMemory(12);

  // Enable the audio shield and set the input and volume
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.volume(0.5);
  audioShield.micGain(40);
  // Configure FFT window function
  myFFT.windowFunction(AudioWindowHanning1024);
  // myFFT.windowFunction(NULL);

  // Initialize BeatDetector
  beatDetector.enableSerialBeatDisplay = false;

  // Initialize servo controls
  for (int i = 0; i < NUM_LIMBS; ++i)
  {
    servoControls[i].initialize();
  }

  // Initialize LEDs
  FastLED.addLeds<NEOPIXEL, 28>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.clearData();

  // Initialize LED ramp
  ledBrightness.go(0, 0, LINEAR, ONCEFORWARD);
}

void loop()
{
  beatDetector.BeatDetectorLoop();

  bool beatDetected = beatDetector.lowBeat;
  // bool beatDetected = false;
  int_least32_t now = millis();

  // Check for beat based on BPM interval
  // if (now > lastBeatTimestamp + BPM_INTERVAL_MS)
  // {
  //   beatDetected = true;
  //   lastBeatTimestamp = millis();
  // }

  if (beatDetected)
  {
    Serial.println("--- BEAT ---");
    Serial.println("start fade in");
    ledBrightness.go(255, LED_FADE_IN_MS, QUADRATIC_IN, ONCEFORWARD);
    isLedOn = true;
    lastBeatTimestamp = millis();

    bpmCalculator.addBeat(millis());
    float bpm = bpmCalculator.calculateBPM();
    Serial.print("BPM: ");
    Serial.println(bpm);

    // Randomly activate or deactivate servos
    bool position[NUM_LIMBS];
    for (int i = 0; i < NUM_LIMBS; ++i)
    {
      position[i] = random(0, 2);
    }

    for (int i = 0; i < NUM_LIMBS; ++i)
    {
      if (position[i])
      {
        servoControls[i].activate();
      }
      else
      {
        servoControls[i].deactivate();
      }
    }
  }

  // Fade out LEDs after a short delay
  if (now > lastBeatTimestamp + LED_FADE_IN_MS && isLedOn)
  {
    float bpm = bpmCalculator.calculateBPM();
    Serial.println("start fade out");
    ledBrightness.go(0, (60000 / bpm) - LED_FADE_IN_MS, QUADRATIC_OUT, ONCEFORWARD);
    isLedOn = false;
  }

  // Update the LED ramp value and set LED brightness
  uint8_t brightness = ledBrightness.update();
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.setBrightness(brightness);
  FastLED.show();
}
