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
  // TODO: Delete all sprites
  for (unsigned i = 0; i < backSprites.size(); ++i) {
    delete backSprites[i];
  }
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
  backSprites(),
  scarySprites(),
  deliciousSprites(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  player("player"),
  hud(),
  score(0),
  numDelSprites(Gamedata::getInstance().getXmlInt("player/numDelSprites")),
  numEnemySprites(Gamedata::getInstance().getXmlInt("player/numEnemySprites")),
  delSpritesDivider(Gamedata::getInstance().getXmlInt("player/delSpritesDivider")),
  enemySpritesDivider(Gamedata::getInstance().getXmlInt("player/enemySpritesDivider"))
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
    backSprites.push_back( new MultiSprite("pinkchar", 0.35) );
    backSprites.push_back( new TwoWaySprite("purpchar", 0.35) );  
  }
  for(int i = 0; i<4; ++i)
  {
    backSprites.push_back( new MultiSprite("pinkchar", 0.6) );
    backSprites.push_back( new TwoWaySprite("purpchar", 0.6) );  
  }
  for(int i = 0; i<8; ++i)
  {
  scarySprites.push_back( new TwoWaySprite("purpchar", 1) );
  }

  for(int i = 0; i<8; ++i)
  {
  deliciousSprites.push_back( new MultiSprite("pinkchar", 1) );
  }

  viewport.setObjectToTrack(&player);
}

void Manager::draw() const {
  world.draw();
  blueb.draw();
  redb.draw();
  layergreensmall.draw();

  for (unsigned int i = 0; i < backSprites.size(); ++i) {
    backSprites[i]->draw();
  }
  for(unsigned int i = 0; i < scarySprites.size(); ++i) {
    scarySprites[i]->draw();
  }
  for(unsigned int i = 0; i < deliciousSprites.size(); ++i) {
    deliciousSprites[i]->draw();
  }

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
  currentSprite = (currentSprite+1) % backSprites.size();
  viewport.setObjectToTrack(backSprites[currentSprite]);
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world.update();
  blueb.update();
  redb.update();
  layergreensmall.update();
  player.update(ticks);
  for (unsigned int i = 0; i < backSprites.size(); ++i) {
    backSprites[i]->update(ticks);
  }
  for(unsigned int i = 0; i < scarySprites.size(); ++i) {
    scarySprites[i]->update(ticks);
    player.checkCollision(scarySprites[i]);
  }
  for(unsigned int i = 0; i < deliciousSprites.size(); ++i) {
    deliciousSprites[i]->update(ticks);
    deliciousSprites[i]->checkCollision(&player); 
    }
  

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
     

          if (keystate[SDLK_9]) {
          switchSprite();
          }

          if (keystate[SDLK_1]) {
          scoreKeeper.setScore(scoreKeeper.getScore() + 100);
          }
          
          if (keystate[SDLK_2]) {
          scoreKeeper.setScore(scoreKeeper.getScore() - 100);
          }
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
