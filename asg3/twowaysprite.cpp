#include "string"
#include "twowaysprite.h"
#include <cmath>
void TwoWaySprite::advanceFrame(Uint32 ticks){
    timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    if(velocityX() >= 0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
    else currentFrame = (numberOfFrames / 2 ) + ((currentFrame+1) % (numberOfFrames/2));
		timeSinceLastFrame = 0;
	}
}
void TwoWaySprite::checkCollision(const Drawable *obj) {
  if( hypot(X()-obj->X(), Y()-obj->Y()) < 20 ) 
  {
  frameWidth = 0.5 * frames[0]->getWidth();
  frameHeight = 0.5 * frames[0]->getHeight();
  }
  else{
  frameWidth = frames[0]->getWidth();
  frameHeight = frames[0]->getHeight();  
  }
}
    TwoWaySprite::TwoWaySprite(const std::string & name) : MultiSprite(name)
{}