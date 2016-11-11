#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <deque>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  started(false), 
  paused(false), 
  sloMo(false), 
  framesAreCapped(Gamedata::getInstance().getXmlBool("framesAreCapped")), 
  frameCap(Gamedata::getInstance().getXmlInt("frameCap")), 
  frames(0), 
  tickSum(0),
  sumOfAllTicks(0),
  timeAtStart(0), timeAtPause(0),
  currTicks(0), prevTicks(0), ticks(0), preTicks(0),
  fpsAveragedOver(Gamedata::getInstance().getXmlInt("fpsAveragedOver")),
  fps(0),
  deck_TicksPerFrame(200,0)  
  {
  start();
  }

Clock::Clock(const Clock& c) :
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  framesAreCapped(c.framesAreCapped), 
  frameCap(c.frameCap), 
  frames(c.frames),
  tickSum(c.tickSum),
  sumOfAllTicks(c.sumOfAllTicks),
  timeAtStart(c.timeAtStart), timeAtPause(c.timeAtPause),
  currTicks(c.currTicks), prevTicks(c.prevTicks), ticks(c.ticks),
  preTicks(c.preTicks),
  fpsAveragedOver(c.fpsAveragedOver),
  fps(c.fps),
  deck_TicksPerFrame(c.deck_TicksPerFrame) 
  {
  start();
}

void Clock::display() const { 

}

void Clock::toggleSloMo() {
  if( started && !sloMo ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    sloMo = true;
  }
  else if ( started && sloMo ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    sloMo = false;
  }
}

unsigned int Clock::getTicks() const { 
  if (paused) return timeAtPause;
  else if ( sloMo ) return 1;
  else return SDL_GetTicks() - timeAtStart; 
}

/* Get ticks since last call */
unsigned int Clock::getElapsedTicks() { 
  if (paused) return 0;
  else if ( sloMo ) return 1;

  currTicks = getTicks();
  ticks = currTicks-prevTicks;
  unsigned int delay = capFrameRate();
  prevTicks = currTicks + delay;
  ticks += delay;
  sumOfAllTicks += ticks;
  return ticks;
}

unsigned int Clock::capFrameRate() const { 
  if ( !framesAreCapped ) return 0u;
  unsigned int delay = std::max(0.0,1000.0/frameCap+0.5 - ticks);
  SDL_Delay( delay );
  return delay;
}


/* Increments frame if Clock is not paused */
Clock& Clock::operator++() { 
  int elapsedticks = 0;

  if ( !paused ) {
    ++frames; 

    elapsedticks = (getTicks() - preTicks);
    tickSum -= deck_TicksPerFrame[0];
    deck_TicksPerFrame.pop_front();
    tickSum += elapsedticks;
    deck_TicksPerFrame.push_back(elapsedticks);

    /* FPS = 1/secPerFrame 
    ** => FPSAvergae = 1/SecPerFrameAverage
    ** => FPSAvergae = 1 / (TotalSec / NumFrames)
    ** => FPSAverage = NumFrames / TotalSec
    ** => FPSAverage = NumFrames / (TotalTicks / 1000)
    ** => FPSAverage = NumFrames * 1000 / TotalTicks
    */
    
    fps = (int) (((float) fpsAveragedOver) * 1000.00f / ((float) tickSum));

    preTicks = getTicks();
  }
  return *this;
}

/* Starts the clock */
void Clock::start() { 
  started = true; 
  paused = false; 
  frames = 0;
  timeAtPause = timeAtStart = SDL_GetTicks(); 
}

void Clock::pause() {
  if( started && !paused ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}

unsigned int Clock::getFPS() const {
  return fps;
}