#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "scoreKeeper.h"
#include "monsterManager.h"
class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  ScoreKeeper& scoreKeeper;
  SDL_Surface * const screen;
  World world;
  World blueb;
  World redb;
  World layergreensmall;

  Viewport& viewport;
  
  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  Player& player;
  MonsterManager& monsterManager;
  Hud hud;
  
  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
