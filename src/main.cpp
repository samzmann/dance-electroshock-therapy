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

#include <Servo.h>

#include "BeatDetector.h"
#include "BpmCalculator.h"
#include "Position.h"
#include "LimbControl.h"

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

LimbControl limbControls[NUM_LIMBS] = {
    LimbControl("Left arm"),
    LimbControl("Right arm"),
    LimbControl("Left leg"),
    LimbControl("Right leg")};

Servo servos[NUM_LIMBS];

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

  servos[0].attach(24);
  servos[1].attach(25);
  servos[2].attach(26);
  servos[3].attach(27);
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

#define BPM 60
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

  if (beatDetected)
  {
    Serial.println("--- BEAT ---");

    bpmCalculator.addBeat(millis());

    float bpm = bpmCalculator.calculateBPM();
    Serial.print("BPM: ");
    Serial.println(bpm);

    bool position[NUM_LIMBS];
    for (int i = 0; i < NUM_LIMBS; ++i)
    {
      position[i] = random(0, 2); // random(0, 2) generates either 0 or 1, which can be interpreted as false or true
    }

    for (int i = 0; i < NUM_LIMBS; ++i)
    {
      if (position[i])
      {
        limbControls[i].activate();
        servos[i].write(90);
        delay(100);
        servos[i].write(0);
      }
    }
  }
}
