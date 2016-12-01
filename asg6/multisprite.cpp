#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <stdlib.h>
#include "explodingSprite.h"
#include <cmath>
#include "scoreKeeper.h"
#include "monsterManager.h"
#include "collStrat.h"
#include "sound.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % (numberOfFrames);
    timeSinceLastFrame = 0;
	}
}

MultiSprite::~MultiSprite()
{
  if(explosion) {
    delete explosion; /* Delete only if program is quit while exploding (multisprite is destroyed while exploding) */
    explosion = NULL;
    }
}

void MultiSprite::incrementScore(unsigned int increment) const
{
  unsigned int newScore = ScoreKeeper::getInstance().getScore()+increment;
  ScoreKeeper::getInstance().setScore(newScore);
  if((newScore != 0) && (newScore%1000) == 0) MonsterManager::getInstance().nextLevel();
}

void MultiSprite::explode()
{
  if(explosion != NULL) return;
  
  if(getName() == "player") {
    if( Player::getInstance().isGod() == true) return; // Don't explode if God Mode
    (SDLSound::getInstance())[1];
  }
  else
  { 
      incrementScore(Gamedata::getInstance().getXmlInt(getName()+"/scoreIncrement"));// * ScoreKeeper::getInstance().getMultiplier() ); This causes problem with next level detector
      (SDLSound::getInstance())[0];
  }
 
  Vector2f v(20,20);
  Sprite A(getName(), getPosition(), v, getFrame());
  explosion = new ExplodingSprite(A); 
  return; 
}


// TODO 3 Fix velocity randomization
MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(rand()%Gamedata::getInstance().getXmlInt("world/width"), 
                    rand()%Gamedata::getInstance().getXmlInt("world/height")), 
           Vector2f(((rand()%100)+120)*((rand()%2) - 1 )+50, // Randomized velocity
                    ((rand()%100)+120)*((rand()%2) - 1 )+50)
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
  destroyed(false),
  currentMode(NORMAL),
  attached(NO)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  explosion(s.explosion),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  destroyed(s.destroyed),
  currentMode(s.currentMode),
  attached(s.attached)
  {}

void MultiSprite::draw() const {
  if(explosion != NULL)
  {
    explosion->draw();
    return;
  } 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

bool MultiSprite::isDestroyed() const{
  return destroyed;
}

void MultiSprite::update(Uint32 ticks) { 
  if(explosion != NULL)
  {
    explosion->update(ticks);
    
    if((explosion->chunkCount()) == 0){
        delete explosion;
        explosion = NULL;
        destroyed = true;
        if(getName() == "player") (SDLSound::getInstance())[3];        
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
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

  /* Code from subjectsprite example */

  float x = X() + getFrame()->getWidth() / 2;
  float y = Y() + getFrame()->getHeight() / 2;
  float px = Player::getInstance().X() + Player::getInstance().getFrame()->getWidth()/2;
  float py = Player::getInstance().Y() + Player::getInstance().getFrame()->getHeight()/2;
  float distanceToPlayer = hypot(x - px, y - py);

  if((currentMode == NORMAL) && (attached == YES))
  {
    if(distanceToPlayer < Gamedata::getInstance().getXmlInt("player/safedistance")) currentMode = EVADE;
  }
  else if ((currentMode == EVADE) && (attached == YES))
  {
    if(distanceToPlayer > Gamedata::getInstance().getXmlInt("player/safedistance")) currentMode = NORMAL;
    else{
      if ( x < px ) goLeft();
      if ( x > px ) goRight();
      if ( y < py ) goUp();
      if ( y > py ) goDown();
    }
  }

}

void MultiSprite::goLeft()  { 
  if (X() > 0) velocityX( -abs(velocityX()) ); 
}
void MultiSprite::goRight() { velocityX( fabs(velocityX()) ); }
void MultiSprite::goUp()    { velocityY( -fabs(velocityY()) ); }
void MultiSprite::goDown()  { velocityY( fabs(velocityY()) ); }


int MultiSprite::getDistance(const Drawable *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}



bool MultiSprite::isNotExploding() const{
  return (explosion == NULL); //(explosion == NULL); /* Returns true if not exploding */
}

/* This only checks for delicious Sprite */
bool MultiSprite::checkCollision(const Drawable * scary)
{
  if(explosion) return destroyed;
  if( scary->isNotExploding() )/* Do this only if scary sprite is NOT exploding )  */
  {
  bool collided = CollStrat::getInstance().execute(*this,*scary);
  if(collided) explode();
  }
  return destroyed;
}
