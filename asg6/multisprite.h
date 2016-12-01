#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
enum ATTACHED{NO, YES};

class ExplodingSprite;
class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite(); 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  
  virtual void explode();
  bool checkCollision(const Drawable * scary);
  bool isDestroyed() const;
  int getDistance(const Drawable*) const;
  void setAttached(const ATTACHED a) {attached = a;}
protected:
  ExplodingSprite * explosion; // Allows us to make explode() const;
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  bool destroyed;
  void incrementScore(unsigned int) const;

  virtual void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&); // Explicityly disallow assignment operator and consturctors
  bool isNotExploding() const;
  enum MODE {NORMAL, EVADE};
  MODE currentMode;
  ATTACHED attached;
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
