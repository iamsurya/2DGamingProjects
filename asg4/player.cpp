#include "string"
#include "player.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void Player::advanceFrame(Uint32 ticks){
    timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    if(velocityX() > 0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
    else if(velocityX() <0) currentFrame = (numberOfFrames / 2 ) + ((currentFrame+1) % (numberOfFrames/2));
		timeSinceLastFrame = 0;
	}
}

    Player::Player(const std::string & name) : MultiSprite(name), accelerationX(0), accelerationY(0)
{}

void Player::update(Uint32 ticks)
{
    advanceFrame(ticks);
    
    /* Handle velocity changes after keypress */
    if(velocityY() != 0) velocityY(velocityY() - (accelerationY) );
    if(velocityX() != 0) velocityX(velocityX() - (accelerationX) );

    /* Update position */
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    
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