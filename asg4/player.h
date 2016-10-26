#ifndef PLAYER__H
#define PLAYER__H
#include "multisprite.h"

class Player : public MultiSprite {
public:
    Player(const std::string &);
    void update(Uint32 ticks);
protected:
void advanceFrame(Uint32 ticks);
};

#endif