#include "hud.h"

Hud::Hud(){}

void Hud::draw() const
{
    const int NO_OF_POINTS = 360;

    SDL_Surface* screen = IOManager::getInstance().getScreen();
  const Uint32 BLUE = SDL_MapRGB(screen->format, 0x00, 0x00, 0xcd);
  IOManager::getInstance().
    printMessageCenteredAt("Using aaline to draw a Circle",20);
  Sint16 x = 320;
  Sint16 y = 240;
  Draw_Pixel(screen, x, y, 0,0,0, 255);
  Sint16 radius = 50;
  Draw_AALine(screen, x+5,y, x+100,y, BLUE);

  int step = 360/NO_OF_POINTS;
  for (int theta = 0; theta < 360; theta += step) {
    Draw_Pixel(screen, x + radius * cos(DegToRad(theta)), 
               y + radius * sin(DegToRad(theta)), 0xff, 0, 0,255);
  }
}