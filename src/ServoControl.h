#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>
#include <Servo.h>

class ServoControl
{
public:
    ServoControl(int pin, int idleAngle, int activeAngle);

    void initialize();
    void activate();
    void deactivate();
    void updateReset();

private:
    Servo servo;
    int pin;
    int angleIdle;
    int angleActive;
    bool activated = false;
    float lastActivateTimestamp;
    const int reset_wait_ms = 300;
};

#endif // SERVO_CONTROL_H