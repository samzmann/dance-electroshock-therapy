#ifndef BPMCALCULATOR_H
#define BPMCALCULATOR_H

class BpmCalculator
{
public:
    BpmCalculator(int historyLength);
    void addBeat(unsigned long timestamp);
    float calculateBPM();

private:
    int beatHistoryLength;
    unsigned long *beatTimes;
};

#endif // BPMCALCULATOR_H