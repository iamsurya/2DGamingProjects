#include "string"
#include "player.h"
#include "viewport.h"
#include "explodingSprite.h"
#include <iostream>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

Player& Player::getInstance()
{
  static Player playerInstance("player");
  return playerInstance;
}

Player::Player(const std::string & name) : TwoWaySprite(name), accelerationX(0), accelerationY(0),
screen(IOManager::getInstance().getScreen()),
aimcolor(SDL_MapRGB(screen->format,Gamedata::getInstance().getXmlInt("player/aimred"),Gamedata::getInstance().getXmlInt("player/aimgreen"),Gamedata::getInstance().getXmlInt("player/aimblue"))),
mousex(0), mousey(0), godMode(false)    
{

  X(Gamedata::getInstance().getXmlInt("player/startLoc/x")); Y(Gamedata::getInstance().getXmlInt("player/startLoc/x")); /* Need to set start location for player's Drawable. */
  velocityX(Gamedata::getInstance().getXmlInt("player/speedX")); /* Start velocity */
  velocityY(Gamedata::getInstance().getXmlInt("player/speedY")); /* can be set in MultiSprite.cpp's constructor, but that makes the code ugly. */
}

void Player::update(Uint32 ticks)
{
  if(explosion)
  {
    explosion->update(ticks);
    if((explosion->chunkCount()) == 0){
        delete explosion;
        explosion = NULL;
        destroyed = true;
    }
    return;
  }

    advanceFrame(ticks);
    
  if (( Y() < 10) && (velocityY() < 0)) {
    velocityY(0);
  }
  if (( Y() > worldHeight-frameHeight-5) && (velocityY() > 0)) {
    velocityY(0);
  }

  if ( (X() < 10) && (velocityX() < 0)){
    velocityX( 0);
  }
  if ( (X() > worldWidth-frameWidth-10) && (velocityX() > 0) ) {
    velocityX(0);
  }  

    /* Handle velocity changes after keypress */
    if(velocityY() != 0) velocityY(velocityY() - (accelerationY) );
    if(velocityX() != 0) velocityX(velocityX() - (accelerationX) );

    /* Update position */
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    
    setPosition(getPosition() + incr);
    
}

// TODO: Stop using magic numbers
void Player::handleMouseEvent(const SDL_MouseMotionEvent *event)
{
  
  mousex = event->x;
  mousey = event->y;
}

/* Help from https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html */
void Player::handleEvent(const SDL_KeyboardEvent *event)
{
  int32_t speed = Gamedata::getInstance().getXmlInt("player/speed");


/* If key is unpressed, move the player */
      if(event->type == SDL_KEYUP) {
        
        /* Up / Down */
        if ( event->keysym.sym == SDLK_s) {
          if(velocityY() > 0) accelerationY = 2;
        }
        if ( event->keysym.sym == SDLK_w  ) {
          if(velocityY() < 0) accelerationY = -2;
        }

        /* Left / Right */
        if ( event->keysym.sym == SDLK_d ) {
          if(velocityX() > 0) accelerationX = 2;
        }
        if ( event->keysym.sym == SDLK_a ) {
          if(velocityX() < 0) accelerationX = -2;
        }
      }
      /* If key is pressed, move the player */
      else if(event->type == SDL_KEYDOWN) {
        
        /* Down */
        if ( event->keysym.sym ==SDLK_s ) {
          velocityY(speed);
          accelerationY = 0;
        }
        /* Up */
        if ( event->keysym.sym ==SDLK_w ) {
          velocityY(speed * -1);
          accelerationY = 0;
        }

        /* Left / Right */
        if ( event->keysym.sym ==SDLK_d ) {
          velocityX(speed);
          accelerationX = 0;
        }
        if ( event->keysym.sym == SDLK_a ) {
          velocityX(speed * -1);
          accelerationX = 0;
        }
      }
}
