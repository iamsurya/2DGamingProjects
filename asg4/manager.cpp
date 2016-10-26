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
  overlay("overlay", Gamedata::getInstance().getXmlInt("overlay/factor")),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  player("player")
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
  sprites.push_back( new TwoWaySprite("purpchar") );
  sprites.push_back( new MultiSprite("pinkchar") );
  }
  sprites.push_back( new Sprite("pizzamonster"));
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
  if(Gamedata::getInstance().getXmlBool("overlay/use")) overlay.draw(1);
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  player.draw();
  clock.display();
  //io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
  //io.printMessageAt("Press T to switch sprites", 10, 45);
  io.printMessageAt(title, 10, 450);
  io.printMessageValueAt("X: ", player.X(), 10, 70);
  io.printMessageValueAt("Y: ", player.Y(), 10, 110);
  io.printMessageValueAt("XV: ", player.velocityX(), 10, 150);
  io.printMessageValueAt("YV: ", player.velocityY(), 10, 190);
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
  overlay.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      
      /* If key is pressed, move the player */
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        
        /* Up / Down */
        if ( keystate[SDLK_s] ) {
          player.velocityY(30);
          //clock.toggleSloMo();
        }
        else if ( keystate[SDLK_w] ) {
          player.velocityY(-30);
        }

        /* Left / Right */
        if ( keystate[SDLK_d] ) {
          player.velocityX(30);
        }
        if ( keystate[SDLK_a] ) {
          player.velocityX(-30);
        }
        
        
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }

      }

      /* When key is unpressed, make velocities 0 */
      if(event.type == SDL_KEYUP) {
        
        player.velocityY(0);
        player.velocityX(0);
        /* Up / Down */
        if ( keystate[SDLK_s] ) {
          player.velocityY(0);
          //clock.toggleSloMo();
        }
        else if ( keystate[SDLK_w] ) {
          player.velocityY(0);
        }

        /* Left / Right */
        if ( keystate[SDLK_d] ) {
          player.velocityX(0);
        }
        if ( keystate[SDLK_a] ) {
          player.velocityX(0);
        }

      }
    }
    draw();
    update();
  }
}
