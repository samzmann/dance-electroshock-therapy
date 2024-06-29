#ifndef LIMB_CONTROL_H
#define LIMB_CONTROL_H

#include <string>

class LimbControl
{
public:
    LimbControl(const std::string &label);
    void activate();

private:
    std::string label;
};

#endif // LIMB_CONTROL_H