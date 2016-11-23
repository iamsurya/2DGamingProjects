#include "monsterManager.h"
#include "gamedata.h"
#include "time.h"
#include "scorekeeper.h"
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
}

MonsterManager::MonsterManager() :
  //backSprites(),
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
  banana(false)
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
  std::cout<<"NEXT LEVEL"<<std::endl;
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
    std::cout<<"Moving "<<*( unusedNames.begin() )<<" to Scary"<<std::endl;
    scaryNames.push_back(( *( unusedNames.begin() ) ));
    unusedNames.pop_front();
    }
    /* DEBUG 
    std::cout<<"\nDelicious: ";
    printNames(deliciousNames);

    std::cout<<"\nScary: ";
    printNames(scaryNames);

    std::cout<<"\nUnused: ";
    printNames(unusedNames);*/

  }
  else
  {
    std::cout<<"YOU WON!!!!OMGOMGOMG"<<std::endl;
  }
}

void MonsterManager::updateMonsterCounts()
{
  /* First update the number of sprites according to score */
  unsigned int score = ScoreKeeper::getInstance().getScore();

  numDelSprites = Gamedata::getInstance().getXmlInt("numDelSprites") + ( score / delSpritesDivider);
  numScarySprites = Gamedata::getInstance().getXmlInt("numEnemySprites") + (score / enemySpritesDivider);

  

  /* Then update the monster lists */
  updateMonsterList(deliciousSprites, deliciousNames, numDelSprites);
  updateMonsterList(scarySprites, scaryNames, numScarySprites);
}

void MonsterManager::updateMonsters(std::list<Drawable *> & itemlist, Uint32 ticks)
{
  std::list<Drawable *>::iterator ptr = itemlist.begin();
  unsigned int x = 0;
  while(ptr != itemlist.end())
  {
    (*ptr)->update(ticks);
    ptr++;
    x++;
    //std::cout<<"Q "<<x<<std::endl;
    if(x > 99) exit(0);
  }
}


void MonsterManager::update(Uint32 ticks)
{
  updateMonsterCounts();

  unsigned int x = 0;
  
  std::list<Drawable *>::iterator ptr = scarySprites.begin();
  
  updateMonsters(deliciousSprites, ticks);

  while(ptr != scarySprites.end())
  {
    //std::cout<<"P "<<(*ptr)->getName()<<" "<<x<<std::endl;
    ptr++;
    x++;

    if(x>99) exit(1);
  }


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
  x = 0;
  while(ptr != scarySprites.end())
  {
    //std::cout<<"C "<<(*ptr)->getName()<<" "<<x<<" "<<player.getName()<<std::endl;
    player.checkCollision(*ptr);
    ptr++;
    x++;
    
    if(x>99) exit(1);
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
  /* delete scarySprites */
  
  deliciousSprites.clear();
  scarySprites.clear();

  deliciousNames.clear();
  scaryNames.clear();
  unusedNames.clear();

  initNameLists();

  ScoreKeeper::getInstance().setScore(100);
  // TODO Reset Player deaths?
}

void MonsterManager::initNameLists()
{
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