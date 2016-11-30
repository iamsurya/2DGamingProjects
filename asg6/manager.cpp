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
#include "sound.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  // MonsterManager deletes its sprites
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  scoreKeeper(ScoreKeeper::getInstance()),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  blueb("blueb", Gamedata::getInstance().getXmlInt("blueb/factor") ),
  redb("redb", Gamedata::getInstance().getXmlInt("redb/factor") ),
  layergreensmall("LayerGreenSmall", Gamedata::getInstance().getXmlInt("LayerGreenSmall/factor") ),
  viewport( Viewport::getInstance() ),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  player(Player::getInstance()),
  monsterManager(MonsterManager::getInstance()),
  hud()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw string("Unable to initialize SDL: ");
  }

  srand (time(NULL));
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  SDLSound::getInstance();

  viewport.setObjectToTrack(&player);
}

void Manager::draw() const {
  world.draw();
  blueb.draw();
  redb.draw();
  layergreensmall.draw();

  monsterManager.draw();
  player.draw();
  clock.display();

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
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();
  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world.update();
  blueb.update();
  redb.update();
  layergreensmall.update();
  monsterManager.update(ticks);
  player.update(ticks);
  hud.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  Uint8 *keystate = NULL;
  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
     

          if (keystate[SDLK_9]) {
          player.explode();
          }
          if (keystate[SDLK_8]) {
          scoreKeeper.setScore(900);
          }
          if (keystate[SDLK_1]) {
          scoreKeeper.setScore(scoreKeeper.getScore() + 100);
          }
          if (keystate[SDLK_2]) {
          scoreKeeper.setScore(scoreKeeper.getScore() - 100);
          }
          if (keystate[SDLK_r]) {
          monsterManager.reset();
          }
      /* Make frames for submission */
      if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      
      /* Quit when Escape is pressed */
      if (keystate[SDLK_ESCAPE]) {
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
    }

    player.handleEvent(&event.key);
    draw();
    update();
  }
}
