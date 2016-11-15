#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "scoreKeeper.h"

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

  std::vector<Drawable*> backSprites; /* In the background, they don't do anything */
  std::vector<Drawable*> scarySprites;  /* These bad creatures will eat us. Avoid them */
  std::vector<Drawable*> deliciousSprites;  /* These delicious items are for us to eat! Muahahaha */

  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  Player player;
  Hud hud;

  unsigned int score;
  unsigned int numDelSprites;
  unsigned int numEnemySprites;
  unsigned int delSpritesDivider;
  unsigned int enemySpritesDivider;
  
  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
