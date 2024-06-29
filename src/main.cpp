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
#include "BpmDetector.h"

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
BpmDetector bpmDetector(8);

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

void loop()
{
  beatDetector.BeatDetectorLoop();

  if (beatDetector.highBeat) // Beat has been detected
  {
    Serial.println("--- BEAT ---");

    bpmDetector.addBeat(millis());

    float bpm = bpmDetector.calculateBPM();
    Serial.print("BPM: ");
    Serial.println(bpm);
  }
}
