#ifndef PLAYER__H
#define PLAYER__H
#include "twowaysprite.h"
#include "gamedata.h"
#include "aaline.h"
#include "ioManager.h"

class ExplodingSprite;

class Player : public TwoWaySprite {
public:
    static Player & getInstance();
    ~Player(){
      //  delete collisionStrategy;
    }
    void update(Uint32 ticks);
    void handleEvent(const SDL_KeyboardEvent *event);
    void handleMouseEvent(const SDL_MouseMotionEvent *event);
    void toggleGodMode() {godMode = !godMode;}
    bool isGod() const {return godMode;}
protected:
int accelerationX;
int accelerationY;
SDL_Surface * screen;
const Uint32 aimcolor;
Sint32 mousex;
Sint32 mousey;

private:
    Player(const std::string &);
    Player(const Player&);
    Player & operator=(const Player&);
    bool godMode;
};

#endif
