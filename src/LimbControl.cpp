#include "LimbControl.h"
#include <Arduino.h>

LimbControl::LimbControl(const std::string &label) : label(label) {}

void LimbControl::activate()
{
    Serial.println(label.c_str());
}