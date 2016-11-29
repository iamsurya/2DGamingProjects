#include "scoreKeeper.h"

 ScoreKeeper& ScoreKeeper::getInstance()
 {
     static ScoreKeeper scoreInstance;
     return scoreInstance;
 }
 unsigned int ScoreKeeper::getScore() const
 {
     return score;
 }
 void ScoreKeeper::setScore(unsigned int s)
 {
     score = s;
 }

ScoreKeeper::ScoreKeeper() : score(0)
{

}