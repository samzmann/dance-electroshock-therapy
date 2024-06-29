#ifndef POSITION_H
#define POSITION_H

class Position

{
public:
    Position(
        int armLeft,
        int armRight,
        int legLeft,
        int legRight,
        char *label, char *stickFigure) : armLeft(armLeft), armRight(armRight), legLeft(legLeft), legRight(legRight), label(*label)
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
    char label;
    char stickFigure;
};

#endif // POSITION_H