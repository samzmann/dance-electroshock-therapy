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

#include "BeatDetector.h"
#include "BpmCalculator.h"
#include "Position.h"
#include "ServoControl.h"

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S audioInput;
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT256 myFFT;
// AudioAnalyzeFFT1024 myFFT;
AudioOutputI2S audioOutput;
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
AudioControlSGTL5000 audioShield;

BeatDetector beatDetector(myFFT);
BpmCalculator bpmCalculator(8);

#define NUM_LIMBS 4

ServoControl servoControls[NUM_LIMBS] = {
    ServoControl(24, 0, 90),
    ServoControl(25, 0, 90),
    ServoControl(26, 0, 90),
    ServoControl(27, 0, 90),
};

#define NUM_LEDS 10
CRGB leds[NUM_LEDS];

void setup()
{
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.volume(0.5);
  audioShield.micGain(40);

  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning1024);
  // myFFT.windowFunction(NULL);

  // Initialize BeatDetector
  beatDetector.enableSerialBeatDisplay = false;

  for (int i = 0; i < NUM_LIMBS; ++i)
  {
    servoControls[i].initialize();
  }

  FastLED.addLeds<NEOPIXEL, 28>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.clearData();
}

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

#define BPM 120
const int BPM_INTERVAL_MS = 60000 / BPM;

int_least32_t lastBpmTimestamp = millis();

void loop()
{

  beatDetector.BeatDetectorLoop();

  // bool beatDetected = beatDetector.highBeat;
  bool beatDetected = false;

  int_least32_t now = millis();

  if (now > lastBpmTimestamp + BPM_INTERVAL_MS)
  {
    beatDetected = true;
    lastBpmTimestamp = millis();
  }

  if (now > lastBpmTimestamp + (BPM_INTERVAL_MS / 2))
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }

  if (beatDetected)
  {
    Serial.println("--- BEAT ---");

    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();

    bpmCalculator.addBeat(millis());

    float bpm = bpmCalculator.calculateBPM();
    Serial.print("BPM: ");
    Serial.println(bpm);

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
}
