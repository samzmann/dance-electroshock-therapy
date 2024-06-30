#include "ServoControl.h"

ServoControl::ServoControl(
    int pin,
    int idleAngle,
    int activeAngle) : pin(pin),
                       angleIdle(idleAngle),
                       angleActive(activeAngle),
                       lastActivateTimestamp(0.0f)
{
}

void ServoControl::initialize()
{
    servo.attach(pin);
    servo.write(angleIdle);
    lastActivateTimestamp = millis();
}

void ServoControl::activate()
{
    servo.write(angleActive);
    activated = true;
    lastActivateTimestamp = millis();
}

void ServoControl::deactivate()
{
    servo.write(angleIdle);
    activated = false;
}

void ServoControl::updateReset()
{
    float now = millis();

    if (activated && (now > lastActivateTimestamp + reset_wait_ms))
    {
        servo.write(angleIdle);
        activated = false;
    }
}