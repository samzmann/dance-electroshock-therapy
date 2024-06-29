#ifndef BPMDETECTOR_H
#define BPMDETECTOR_H

class BpmDetector
{
public:
    BpmDetector(int historyLength);
    void addBeat(unsigned long timestamp);
    float calculateBPM();

private:
    int beatHistoryLength;
    unsigned long *beatTimes;
};

#endif // BPMDETECTOR_H