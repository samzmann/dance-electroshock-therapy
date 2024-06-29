#include "BpmCalculator.h"
#include <Arduino.h>

BpmCalculator::BpmCalculator(int historyLength) : beatHistoryLength(historyLength)
{
    // This line dynamically allocates an array of unsigned long integers with a size of beatHistoryLength and initializes all elements to zero.
    beatTimes = new unsigned long[beatHistoryLength]();
}

void BpmCalculator::addBeat(unsigned long timestamp)
{
    // Shift the beatTimes array to the left
    for (int i = 0; i < beatHistoryLength - 1; i++)
    {
        beatTimes[i] = beatTimes[i + 1];
    }

    // Add the new timestamp at the end of the array
    beatTimes[beatHistoryLength - 1] = timestamp;
}

float BpmCalculator::calculateBPM()
{
    if (beatTimes[1] == 0)
        return 0.0; // Not enough data to calculate BPM

    unsigned long totalTime = 0;

    for (int i = 1; i < beatHistoryLength; i++)
    {
        totalTime += beatTimes[i] - beatTimes[i - 1];
    }

    float averageTime = totalTime / (float)(beatHistoryLength - 1);
    return 60000.0 / averageTime; // Convert to BPM
}