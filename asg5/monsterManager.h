#include "twowaysprite.h"
#include "player.h"

class MonsterManager
{
public:
    static MonsterManager& getInstance();
    void update(Uint32);
    void draw();
private:
    MonsterManager();
    MonsterManager(MonsterManager &);
    MonsterManager & operator=(MonsterManager &);
    
    std::vector<Drawable*> backSprites; /* In the background, they don't do anything */
  std::vector<Drawable*> scarySprites;  /* These bad creatures will eat us. Avoid them */
  std::vector<Drawable*> deliciousSprites;  /* These delicious items are for us to eat! Muahahaha */
  std::vector<std::string> unusedNames;
  std::vector<std::string> deliciousNames;
  std::vector<std::string> scaryNames;

  unsigned int numDelSprites;
  unsigned int numEnemySprites;
  unsigned int delSpritesDivider;
  unsigned int enemySpritesDivider;

  Player& player;
};