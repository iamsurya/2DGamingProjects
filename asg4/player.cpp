#include "string"
#include "player.h"

void Player::advanceFrame(Uint32 ticks){
    timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    if(velocityX() >= 0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
    else currentFrame = (numberOfFrames / 2 ) + ((currentFrame+1) % (numberOfFrames/2));
		timeSinceLastFrame = 0;
	}
}

    Player::Player(const std::string & name) : MultiSprite(name)
{}

void Player::update(Uint32 ticks)
{
    advanceFrame(ticks);
    
    /* Update position */
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    
std::cout<<"Vx"<<velocityX()<<" Vy"<<velocityY()<<std::endl;
  /*if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  */
}