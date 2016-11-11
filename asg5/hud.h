#include "ioManager.h"
#include "aaline.h"
#include "gamedata.h"
#include <cmath>  // for sqrtf

class Hud
{
public:
    Hud();
    void draw() const;
    void update();
    void setshow(bool s) {show = s;}
    void toggleshow() {show = !show;}
private:
    const Gamedata& gdata;
    SDL_Surface * screen;

    const int TOPLINE;
    const int BOTLINE;
    const Uint32 LINECOLOR;
    bool show;
    unsigned int timetoshow;
    //const Uint32 BLUE;


   // int hudWidth;
   // int hudHeight;
};