#ifndef PLAYER__H
#define PLAYER__H
#include "multisprite.h"
#include "gamedata.h"

class Player : public MultiSprite {
public:
    Player(const std::string &);
    void update(Uint32 ticks);
    void handleEvent(const SDL_KeyboardEvent *event);
protected:
void advanceFrame(Uint32 ticks);
int accelerationX;
int accelerationY;
};

#endif
