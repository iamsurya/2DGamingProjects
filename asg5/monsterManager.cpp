#include "monsterManager.h"
#include "gamedata.h"
#include "time.h"
#include "scoreKeeper.h"
#include "ioManager.h"
#include "collStrat.h"
/* Stackoverflow for converting integer to string so that I can read monster names from XML */
/* http://stackoverflow.com/a/4668799/4734557 */
template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

MonsterManager & MonsterManager::getInstance()
{
    static MonsterManager monsterManagerInstance;
    return monsterManagerInstance;
}

MonsterManager::~MonsterManager()
{
  deleteSprites(deliciousSprites);
  deleteSprites(scarySprites);
  CollStrat::getInstance().deleteStrategy();
}

MonsterManager::MonsterManager() :
  scarySprites(),
  deliciousSprites(),
  unusedNames(),
  deliciousNames(),
  scaryNames(),
  numDelSprites(Gamedata::getInstance().getXmlInt("numDelSprites")),
  numScarySprites(Gamedata::getInstance().getXmlInt("numEnemySprites")),
  delSpritesDivider(Gamedata::getInstance().getXmlInt("delSpritesDivider")),
  enemySpritesDivider(Gamedata::getInstance().getXmlInt("enemySpritesDivider")),
  player(Player::getInstance()),
  winText(" ")
{
  srand(time(NULL));
  initNameLists();
}

void MonsterManager::drawMonsters(std::list<Drawable *> & itemlist)
{
  std::list<Drawable *>::const_iterator ptr = itemlist.begin();

  while(ptr != itemlist.end())
  {
    (*ptr)->draw();
    ptr++;
  }
}

void MonsterManager::draw()
{
  drawMonsters(deliciousSprites);
  drawMonsters(scarySprites);
}

void MonsterManager::updateMonsterList(std::list<Drawable *> & itemlist, std::list<string> & nameslist, unsigned int maxsize)
{
  if(nameslist.size() > 0) /* Make sure scary list is not empty */
  {
    /* First make sure enough delicious and scary monsters exist */
    while(itemlist.size() < maxsize)
    {
      /* First pick a sprite name to create */
      unsigned int i =  rand() % nameslist.size();

      /* Find the string for that number from Sprite Names */
      std::list<string>::const_iterator stringiterator = nameslist.begin();
      for(unsigned int j = 0; j < i  ; j ++) stringiterator++;

      /* Push this sprite type to deliciousSprites */
      itemlist.push_back(new TwoWaySprite(*stringiterator));
    }
  }
}

void MonsterManager::printNames(std::list<std::string> & nameList) const
{
  std::list<std::string>::const_iterator nameptr = nameList.begin();

while(nameptr != nameList.end())
  {
    std::cout<<*nameptr<<std::endl;
    nameptr++;
  }

}

void MonsterManager::printNames(std::list<Drawable *> & nameList) const
{
  std::list<Drawable *>::const_iterator nameptr = nameList.begin();

while(nameptr != nameList.end())
  {
    std::cout<<(*nameptr)->getName()<<std::endl;
    nameptr++;
  }

}

void MonsterManager::nextLevel()
{
  if(scaryNames.size() > 0)
  {
    /* Move all scarySprites that match the first scaryName to deliciousSprites */
    std::list<std::string>::const_iterator nameptr = scaryNames.begin();
    std::list<Drawable *>::iterator ptr = scarySprites.begin();

    while(ptr != scarySprites.end())
    {
      /* if sprite name matches, move it to deliciousSprites */ 
      if( (*nameptr) == ( (*ptr)->getName() ) )
      {
        deliciousSprites.push_back(*ptr); /* Push back stores copy of value? */
        ptr = scarySprites.erase(ptr);
      }
      else ptr++;
    } 

    // Move the scaryName to deliciousnames
    deliciousNames.push_back(*nameptr);
    scaryNames.pop_front();
    
    
    if( unusedNames.size() > 0 )
    {
    // Move an Unused Name to scaryName
    scaryNames.push_back(( *( unusedNames.begin() ) ));
    unusedNames.pop_front();
    }

    if(scaryNames.size() == 0)
    {
        winText = Gamedata::getInstance().getXmlStr("tophud/winText/text");//"YOU WON!";
    }
    /* DEBUG 
    std::cout<<"\nDelicious: ";
    printNames(deliciousNames);

    std::cout<<"\nScary: ";
    printNames(scaryNames);

    std::cout<<"\nUnused: ";
    printNames(unusedNames);*/

  }
}

void MonsterManager::updateMonsterCounts()
{
  /* First update the number of sprites according to score */
  unsigned int score = ScoreKeeper::getInstance().getScore();

  numDelSprites = (numDelSprites < (unsigned int) Gamedata::getInstance().getXmlInt("maxDelSprites")) ? Gamedata::getInstance().getXmlInt("numDelSprites") + ( score / delSpritesDivider) : numDelSprites;
  numScarySprites = (numScarySprites < (unsigned int) Gamedata::getInstance().getXmlInt("maxScarySprites")) ? Gamedata::getInstance().getXmlInt("numEnemySprites") + (score / enemySpritesDivider) : numScarySprites;

  

  /* Then update the monster lists */
  updateMonsterList(deliciousSprites, deliciousNames, numDelSprites);
  updateMonsterList(scarySprites, scaryNames, numScarySprites);
}

void MonsterManager::updateMonsters(std::list<Drawable *> & itemlist, Uint32 ticks)
{
  std::list<Drawable *>::iterator ptr = itemlist.begin();
  while(ptr != itemlist.end())
  {
    (*ptr)->update(ticks);
    ptr++;
  }
}


void MonsterManager::update(Uint32 ticks)
{
  updateMonsterCounts();
  
  std::list<Drawable *>::iterator ptr = scarySprites.begin();
  
  updateMonsters(deliciousSprites, ticks);
  updateMonsters(scarySprites, ticks);

  /* Check for explosions now and eaten sprites*/
  /* deliciousSprites */
  ptr = deliciousSprites.begin();

  while(ptr != deliciousSprites.end())
  {
    /* Checks collision and explodes the babies */
    if( !((*ptr)->checkCollision(&player)) ) ptr++;
    else {  // Delete the sprite from the list if its finished exploding.
          delete (*ptr);
          ptr = deliciousSprites.erase(ptr);
    }
  }

  /* scarySprites should hurt the player */ 
  ptr = scarySprites.begin();
  while(ptr != scarySprites.end())
  {
    player.checkCollision(*ptr);
    ptr++;
  }

}

void MonsterManager::deleteSprites(std::list<Drawable*> & list)
{
  std::list<Drawable *>::iterator ptr = list.begin();
  while(ptr != list.end())
  {
    delete (*ptr);
    ptr++;
  }
}

void MonsterManager::reset()
{
  
  /* delete all sprites */
  deleteSprites(deliciousSprites);
  deleteSprites(scarySprites);
  
  deliciousSprites.clear();
  scarySprites.clear();

  deliciousNames.clear();
  scaryNames.clear();
  unusedNames.clear();

  initNameLists();
  winText = " ";
  ScoreKeeper::getInstance().setScore(0);
  // TODO Reset Player deaths?
}

void MonsterManager::initNameLists()
{
  if(Gamedata::getInstance().getXmlInt("numMonstersNames") < (Gamedata::getInstance().getXmlInt("numInitialDeliciousNames") + Gamedata::getInstance().getXmlInt("numInitialScaryNames")))
  {
    std::cout<<"\nError: Check XML file values. numMonstersNames should be greater or equal to (numInitialDeliciousNames + numInitialScaryNames)\n"<<std::endl;
    exit(0);
  }
  /* Populate vector of unusedNames with names from XML file based on number of monsters */
  for(int i = 0; i<Gamedata::getInstance().getXmlInt("numMonstersNames"); i++)
  { 
    unusedNames.push_back("Monster"+ toString(i));
  }

  /* Load deliciousNames with first N names */
  for(int i = 0; i<Gamedata::getInstance().getXmlInt("numInitialDeliciousNames"); i++)
  { 
    deliciousNames.push_back( *( unusedNames.begin() ) );
    unusedNames.pop_front();
  }

  /* Load scaryNames with next Y names */
  for(int i = 0; i<Gamedata::getInstance().getXmlInt("numInitialScaryNames"); i++)
  { 
    scaryNames.push_back( *( unusedNames.begin() ) );
    unusedNames.pop_front();
  }
}

const std::list<std::string> * MonsterManager::getScaryNames() const {
return (&scaryNames);
}