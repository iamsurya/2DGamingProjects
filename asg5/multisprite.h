#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class ExplodingSprite;

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&, double z = 1.0f);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  
  void explode();

  int getDistance(const Drawable*) const;

protected:
  ExplodingSprite * explosion; 
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  double zoom;

  virtual void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&); // Explicityly disallow assignment operator and consturctors

};
#endif