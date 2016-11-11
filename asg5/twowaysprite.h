#ifndef TWOWAYSPRITE__H
#define TWOWAYSPRITE__H
#include "multisprite.h"

class TwoWaySprite : public MultiSprite {
public:
    TwoWaySprite(const std::string &, double zoom = 1.0f);
protected:
void advanceFrame(Uint32 ticks);
};

#endif