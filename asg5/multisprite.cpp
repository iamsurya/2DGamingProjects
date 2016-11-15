#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <stdlib.h>
#include "explodingSprite.h"
#include <cmath>
#include "scoreKeeper.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % (numberOfFrames);
    timeSinceLastFrame = 0;
	}
}

void MultiSprite::explode()
{
  if(explosion) return;
  ScoreKeeper::getInstance().setScore(ScoreKeeper::getInstance().getScore()+100);
  // TODO : Also change the X and Y location of the sprite
  //Sprite * A = new Sprite(getName(), getPosition(), getVelocity(), getFrame());
  Vector2f *v = new Vector2f(20,20);
  Sprite * A = new Sprite(getName(), getPosition(), *v, getFrame());
  explosion = new ExplodingSprite(*A);
  delete v;
  delete A;
}

MultiSprite::MultiSprite( const std::string& name, double z) :
  Drawable(name, 
           Vector2f(rand()%2000,//Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    rand()%2000),//Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(((rand()%100)+120)*((rand()%2) - 1 )+50,//*Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    ((rand()%100)+120)*((rand()%2) - 1 )+50)//*Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  zoom(z), collisionStrategy(new PerPixelCollisionStrategy)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  explosion(NULL),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  zoom(s.zoom), collisionStrategy(s.collisionStrategy)
  { }

void MultiSprite::draw() const {
  if(explosion)
  {
    explosion->draw();
    return;
  } 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y, 0, zoom);
}

void MultiSprite::update(Uint32 ticks) { 
  
  if(explosion)
  {
    explosion->update(ticks);
    
    if((explosion->chunkCount()) == 0){
        delete explosion;
        explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001 * zoom;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
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
  }  

}

int MultiSprite::getDistance(const Drawable *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

/* Player thinks bigger microbes are scary, delicious microbes think player is scary */
bool MultiSprite::checkCollision(const Drawable * scary)
{
  bool collided = collisionStrategy->execute(*this, *scary);
  if(collided) explode();
  return collided;
}