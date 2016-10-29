#include "ioManager.h"
#include "aaline.h"
#include <cmath>  // for sqrtf

#ifndef PI
#define PI 3.1415926535897
#endif

#define RAD2DEG (180/PI)
#define DEG2RAD (PI/180)

#define DegToRad(x) ((x)*DEG2RAD)
#define RadToDeg(x) ((x)*RAD2DEG)

class Hud
{
public:
    Hud();
    void draw() const;
    void update();

private:
  //  const IOManager& io;
   // int hudWidth;
   // int hudHeight;
};