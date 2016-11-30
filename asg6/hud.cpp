#include "hud.h"
#include "clock.h"
#include "scoreKeeper.h"
#include "monsterManager.h"
#include "multisprite.h"
#include "sprite.h"
#include "viewport.h"

Hud::Hud(): gdata(Gamedata::getInstance()),
            screen(IOManager::getInstance().getScreen()),
            TOPLINE(gdata.getXmlInt("tophud/lineYloc")), BOTLINE(gdata.getXmlInt("bothud/lineYloc")),
            LINECOLOR(SDL_MapRGB(screen->format, gdata.getXmlInt("tophud/linered"), gdata.getXmlInt("tophud/linegreen"), gdata.getXmlInt("tophud/lineblue"))),
            show(true), timetoshow((unsigned int) gdata.getXmlInt("tophud/showtime")),
            scaryNames(MonsterManager::getInstance().getScaryNames())
             {}


void Hud::printSpritesToAvoid() const{
  Uint32 x = 0; 
  Uint32 y = 0; 
  
  std::list<std::string>::const_iterator nameptr = scaryNames->begin();
  unsigned int count = 1;
  while(nameptr != scaryNames->end())
  {
    /* Create a multisprite from the name */
    MultiSprite A(*nameptr);
   
    /* Figure out where to draw this sprite */
    x = gdata.getXmlInt("avoidtext/xloc")+ (count*gdata.getXmlInt("avoidtext/spriteXIncrement")); 
    x += Viewport::getInstance().X() + (A.getFrame()->getWidth() / 4 );
    y = 0;
    y += Viewport::getInstance().Y() + (A.getFrame()->getHeight() / 6 );
    Vector2f pos(x,y);
    Vector2f vel(0,0);
   
    /* Create a regulat sprite b/c MultiSprite doesn't have a constructor that takes velocity and position */
    Sprite B(A.getName(), pos, vel, A.getFrame());
    B.draw();

    nameptr++;
    count++;
  }
}

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

  /* Show the Score */
  IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("scoretext/text") + " ", ScoreKeeper::getInstance().getScore(), gdata.getXmlInt("scoretext/xloc"), gdata.getXmlInt("scoretext/yloc"));

  /* Show that you've won */
  IOManager::getInstance().
      printMessageCenteredAt(MonsterManager::getInstance().getWinText(), gdata.getXmlInt("tophud/winText/y"),2);
    
  /* Bottom HUD things to show */
  IOManager::getInstance().printMessageAt(gdata.getXmlStr("screenTitle"), 10, 450);

  printSpritesToAvoid();

  if(show)
  {

    // Top HUD
    IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("fpstext/text") + " ", Clock::getInstance().getFPS(), gdata.getXmlInt("fpstext/loc/x"), gdata.getXmlInt("fpstext/loc/y"));

    IOManager::getInstance()
      .printMessageValueAt("Elapsed Seconds: ", Clock::getInstance().getTotalTicks()/1000, gdata.getXmlInt("fpstext/loc/x") + 60, gdata.getXmlInt("fpstext/loc/y"));

    
    IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("tophud/scaryNum/text"), MonsterManager::getInstance().getMaxScary(),
                            gdata.getXmlInt("tophud/scaryNum/x"), gdata.getXmlInt("tophud/scaryNum/y"));

    IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("tophud/delAct/text"), MonsterManager::getInstance().getNumDelicious(),
                            gdata.getXmlInt("tophud/delAct/x"), gdata.getXmlInt("tophud/delAct/y"));
    IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("tophud/delNum/text"), MonsterManager::getInstance().getMaxDelicious(),
                            gdata.getXmlInt("tophud/delNum/x"), gdata.getXmlInt("tophud/delNum/y"));
        
    
    IOManager::getInstance()
      .printMessageValueAt(gdata.getXmlStr("tophud/scaryAct/text"), MonsterManager::getInstance().getNumScary(),
                            gdata.getXmlInt("tophud/scaryAct/x"), gdata.getXmlInt("tophud/scaryAct/y"));


    // Bottom HUD
    Draw_AALine(screen, 0,BOTLINE, screen->w,BOTLINE, 1, LINECOLOR);
    
    Draw_AALine(screen, 0,BOTLINE+3, screen->w,BOTLINE+3, 1, LINECOLOR);

    Draw_Circle(screen, gdata.getXmlInt("avoidcircle/xloc"), gdata.getXmlInt("avoidcircle/yloc"), gdata.getXmlInt("avoidcircle/radius"),
                gdata.getXmlInt("avoidcircle/thick"), gdata.getXmlInt("avoidcircle/red"), gdata.getXmlInt("avoidcircle/green"), gdata.getXmlInt("avoidcircle/blue"),
                gdata.getXmlInt("avoidcircle/alpha"));

    IOManager::getInstance()
      .printMessageAt(gdata.getXmlStr("goaltext/text"), gdata.getXmlInt("goaltext/xloc"), gdata.getXmlInt("goaltext/yloc"));

    IOManager::getInstance()
      .printMessageAt(gdata.getXmlStr("helptext/text"), gdata.getXmlInt("helptext/xloc"), gdata.getXmlInt("helptext/yloc"));

  }
}

void Hud::update()
{
  unsigned int totalticks = Clock::getInstance().getTotalTicks(); 
  if( (totalticks > timetoshow) && (totalticks < (timetoshow + 100))) show = false;
}