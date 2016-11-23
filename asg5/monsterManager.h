#include "twowaysprite.h"
#include "player.h"
#include <list>
class MonsterManager
{
public:
    static MonsterManager& getInstance();
    void update(Uint32);
    void draw();
    void nextLevel();
    unsigned int getNumDelicious() const {return deliciousSprites.size();}
    unsigned int getNumScary() const {return scarySprites.size();}
    unsigned int getMaxDelicious() const {return numDelSprites;}
    unsigned int getMaxScary() const {return numScarySprites;}
    void reset();
    std::string getWinText() const {return winText;}
private:
    ~MonsterManager();
    void initNameLists();
    MonsterManager();
    MonsterManager(MonsterManager &);
    MonsterManager & operator=(MonsterManager &);
    void drawMonsters(std::list<Drawable *> &);
    void updateMonsters(std::list<Drawable *> &, Uint32);
    void updateMonsterList(std::list<Drawable *> &, std::list<string> &, unsigned int);
    void updateMonsterCounts();
    void printNames(std::list<std::string> &) const;
    void printNames(std::list<Drawable*> &) const;
    void deleteSprites(std::list<Drawable*> &);
    //std::list<Drawable*> backSprites; /* In the background, they don't do anything */
    std::list<Drawable*> scarySprites;  /* These bad creatures will eat us. Avoid them */
    std::list<Drawable*> deliciousSprites;  /* These delicious items are for us to eat! Muahahaha */
    std::list<std::string> unusedNames;
    std::list<std::string> deliciousNames;
    std::list<std::string> scaryNames;

    unsigned int numDelSprites;
    unsigned int numScarySprites;
    unsigned int delSpritesDivider;
    unsigned int enemySpritesDivider;

    Player& player;
    std::string winText;
};