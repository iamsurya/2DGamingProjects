#include "string"
#include "twowaysprite.h"

void TwoWaySprite::advanceFrame(Uint32 ticks){
    timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    if(velocityX() >= 0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
    else currentFrame = (numberOfFrames / 2 ) + ((currentFrame+1) % (numberOfFrames/2));
		timeSinceLastFrame = 0;
	}
}

    TwoWaySprite::TwoWaySprite(const std::string & name, double zoom) : MultiSprite(name,zoom)
{}