#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position
{
public:
    Position(
        int armLeft,
        int armRight,
        int legLeft,
        int legRight) : armLeft(armLeft), armRight(armRight), legLeft(legLeft), legRight(legRight)
    {
        coords[0] = &this->armLeft;
        coords[1] = &this->armRight;
        coords[2] = &this->legLeft;
        coords[3] = &this->legRight;
    }

    int &armLeft;
    int &armRight;
    int &legLeft;
    int &legRight;
    int *coords[4];
};

#endif // POSITION_H