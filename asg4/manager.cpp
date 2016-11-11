#include <iostream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include "multisprite.h"
#include "twowaysprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "player.h"


Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  blueb("blueb", Gamedata::getInstance().getXmlInt("blueb/factor") ),
  redb("redb", Gamedata::getInstance().getXmlInt("redb/factor") ),
  layergreensmall("LayerGreenSmall", Gamedata::getInstance().getXmlInt("LayerGreenSmall/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  player("player"),
  hud()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }

  srand (time(NULL));
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  /* Sprite * A = new Sprite("star");
  Sprite B = new Sprite("greenorb"); 
  B = A; */
  for(int i = 0; i<4; ++i)
  {
    sprites.push_back( new MultiSprite("pinkchar", 0.35) );
    sprites.push_back( new TwoWaySprite("purpchar", 0.35) );  
  }
  for(int i = 0; i<4; ++i)
  {
    sprites.push_back( new MultiSprite("pinkchar", 0.6) );
    sprites.push_back( new TwoWaySprite("purpchar", 0.6) );  
  }
  for(int i = 0; i<4; ++i)
  {
  sprites.push_back( new TwoWaySprite("purpchar", 1) );
  sprites.push_back( new MultiSprite("pinkchar", 1) );
  }
  
  //sprites.push_back( new Sprite("greenorb") );
  player.velocityX(0);
  player.velocityY(0);
  viewport.setObjectToTrack(&player);
}

void Manager::draw() const {
  world.draw();
  blueb.draw();
  redb.draw();
  layergreensmall.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  player.draw();
  clock.display();
  //io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
  //io.printMessageAt("Press T to switch sprites", 10, 45);
  
/*  io.printMessageValueAt("X: ", player.X(), 10, 70);
  io.printMessageValueAt("Y: ", player.Y(), 10, 110);
  io.printMessageValueAt("XV: ", player.velocityX(), 10, 150);
  io.printMessageValueAt("YV: ", player.velocityY(), 10, 190);*/
  hud.draw();
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
  //viewport.setObjectToTrack(&player);
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world.update();
  blueb.update();
  redb.update();
  layergreensmall.update();
  player.update(ticks);
  hud.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  //SDL_KeyboardEvent keyEvent;
  bool done = false;
  Uint8 *keystate = NULL;
  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
     
      /* Make frames for submission */
      if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      
      /* Quit when Escape is pressed */
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }

      /* Pause game if p is pressed */
      if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
      
      if ( keystate[SDLK_F1] ) {
          hud.toggleshow();
        }

      /* Ask the player class to handle keyboard presses */
      
    }

    player.handleEvent(&event.key);
    player.handleMouseEvent(&event.motion);
    draw();
    update();
  }
}
