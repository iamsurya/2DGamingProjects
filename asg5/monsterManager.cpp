#include "monsterManager.h"
#include "gamedata.h"

MonsterManager & MonsterManager::getInstance()
{
    static MonsterManager monsterManagerInstance;
    return monsterManagerInstance;
}

MonsterManager::MonsterManager() :
  backSprites(),
  scarySprites(),
  deliciousSprites(),
  unusedNames(),
  deliciousNames(),
  scaryNames(),
  numDelSprites(Gamedata::getInstance().getXmlInt("player/numDelSprites")),
  numEnemySprites(Gamedata::getInstance().getXmlInt("player/numEnemySprites")),
  delSpritesDivider(Gamedata::getInstance().getXmlInt("player/delSpritesDivider")),
  enemySpritesDivider(Gamedata::getInstance().getXmlInt("player/enemySpritesDivider")),
  player(Player::getInstance())
{
    for(unsigned int i = 0; i < 20; i ++)
    {deliciousSprites.push_back(new TwoWaySprite("Char5",1));
    deliciousSprites.push_back(new TwoWaySprite("Char6",1));
    }
}

void MonsterManager::draw()
{

  for (unsigned int i = 0; i < backSprites.size(); ++i) {
    backSprites[i]->draw();
  }
  for(unsigned int i = 0; i < scarySprites.size(); ++i) {
    scarySprites[i]->draw();
  }
  for(unsigned int i = 0; i < deliciousSprites.size(); ++i) {
    deliciousSprites[i]->draw();
  }
}

void MonsterManager::update(Uint32 ticks)
{
  for (unsigned int i = 0; i < backSprites.size(); ++i) {
    backSprites[i]->update(ticks);
  }
  for(unsigned int i = 0; i < scarySprites.size(); ++i) {
    scarySprites[i]->update(ticks);
    player.checkCollision(scarySprites[i]);
  }
  for(unsigned int i = 0; i < deliciousSprites.size(); ++i) {
    deliciousSprites[i]->update(ticks);
    deliciousSprites[i]->checkCollision(&player); 
    }
}
