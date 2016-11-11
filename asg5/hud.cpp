#include "hud.h"
#include "clock.h"
Hud::Hud(): gdata(Gamedata::getInstance()),
            screen(IOManager::getInstance().getScreen()),
            TOPLINE(gdata.getXmlInt("tophud/lineYloc")), BOTLINE(gdata.getXmlInt("bothud/lineYloc")),
            LINECOLOR(SDL_MapRGB(screen->format, gdata.getXmlInt("tophud/linered"), gdata.getXmlInt("tophud/linegreen"), gdata.getXmlInt("tophud/lineblue"))),
            show(true), timetoshow((unsigned int) gdata.getXmlInt("tophud/showtime"))
             {}

void Hud::draw() const
{

  /* Top HUD always shows name and information */
  Draw_AALine(screen, 0,TOPLINE, screen->w,TOPLINE, 1, LINECOLOR);
  Draw_AALine(screen, 0,TOPLINE+3, screen->w,TOPLINE+3, 1, LINECOLOR);
  /* Name of Game in the center */
  IOManager::getInstance().
    printMessageCenteredAt(gdata.getXmlStr("tophud/gamename").c_str(), gdata.getXmlInt("tophud/titleyloc"), 2);

  /* Show text for enemies to avoid */
  IOManager::getInstance().printMessageAt(gdata.getXmlStr("avoidtext/text").c_str(), gdata.getXmlInt("avoidtext/xloc"), gdata.getXmlInt("avoidtext/yloc"), 0);


  /* Bottom HUD things to show */
  IOManager::getInstance().printMessageAt(gdata.getXmlStr("screenTitle"), 10, 450);

  if(show)
  {

    // Top HUD
    IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("fpstext/text") + " ", Clock::getInstance().getFPS(), gdata.getXmlInt("fpstext/loc/x"), gdata.getXmlInt("fpstext/loc/y"));

    IOManager::getInstance()
      .printMessageValueAt("Elapsed Seconds: ", Clock::getInstance().getTotalTicks()/1000, gdata.getXmlInt("fpstext/loc/x") + 60, gdata.getXmlInt("fpstext/loc/y"));


    // Bottom HUD
    Draw_AALine(screen, 0,BOTLINE, screen->w,BOTLINE, 1, LINECOLOR);
    
    Draw_AALine(screen, 0,BOTLINE+3, screen->w,BOTLINE+3, 1, LINECOLOR);

    IOManager::getInstance()
      .printMessageAt(gdata.getXmlStr("helptext/text"), gdata.getXmlInt("helptext/xloc"), gdata.getXmlInt("helptext/yloc"));

  }
}

void Hud::update()
{
  unsigned int totalticks = Clock::getInstance().getTotalTicks(); 
  if( (totalticks > timetoshow) && (totalticks < (timetoshow + 100))) show = false;
}