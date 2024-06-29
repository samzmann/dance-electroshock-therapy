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
#include "BeatDetector.h"
#include "BpmCalculator.h"
#include "Position.h"

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
}

Position positions[] = {
    Position(0, 0, 0, 0),
    Position(1, 0, 0, 0),
    Position(0, 1, 0, 0),
    Position(1, 1, 0, 0),
    Position(0, 0, 1, 0),
    Position(0, 0, 0, 1),
    Position(0, 0, 1, 1),
    Position(1, 0, 1, 0),
    Position(0, 1, 0, 1),
    Position(0, 1, 1, 0),
    Position(1, 0, 0, 1),
    Position(1, 1, 1, 0),
    Position(1, 1, 0, 1),
    Position(1, 0, 1, 1),
    Position(0, 1, 1, 1),
    Position(1, 1, 1, 1)};

void loop()
{
  beatDetector.BeatDetectorLoop();

  if (beatDetector.highBeat) // Beat has been detected
  {
    Serial.println("--- BEAT ---");

    bpmCalculator.addBeat(millis());

    float bpm = bpmCalculator.calculateBPM();
    Serial.print("BPM: ");
    Serial.println(bpm);

    Serial.println("Move");

    int position[4];
    for (int i = 0; i < 4; i++)
    {
      position[i] = random(2); // Generates either 0 or 1
    }

    Serial.print("Position: ");
    Serial.print("ArmLeft: ");
    Serial.print(position[0]);
    Serial.print(", ArmRight: ");
    Serial.print(position[1]);
    Serial.print(", LegLeft: ");
    Serial.print(position[2]);
    Serial.print(", LegRight: ");
    Serial.println(position[3]);

    for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); i++)
    {
      if (positions[i].armLeft == position[0] &&
          positions[i].armRight == position[1] &&
          positions[i].legLeft == position[2] &&
          positions[i].legRight == position[3])
      {
        Serial.print("Position index: ");
        Serial.println(i);
        break;
      }
    }
  }
}
