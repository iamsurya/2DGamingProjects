#ifndef PLAYER__H
#define PLAYER__H
#include "multisprite.h"
#include "gamedata.h"
#include "aaline.h"
#include "ioManager.h"

class ExplodingSprite;

class Player : public MultiSprite {
public:
    Player(const std::string &);
    ~Player(){
        delete collisionStrategy;
    }
    void draw() const;
    void update(Uint32 ticks);
    void handleEvent(const SDL_KeyboardEvent *event);
    void handleMouseEvent(const SDL_MouseMotionEvent *event);
    void explode();
protected:
void advanceFrame(Uint32 ticks);
int accelerationX;
int accelerationY;
SDL_Surface * screen;
const Uint32 aimcolor;
Sint32 mousex;
Sint32 mousey;
mutable bool drawline;

private:
    Player(const Player&);
    Player & operator=(const Player&);
};

#endif
