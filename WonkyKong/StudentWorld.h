#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"

#include <string>
#include <vector>
#include <list>

class Actor;
class Player;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  Level* getLevel() const;
  ~StudentWorld();
private:
	std::list<Actor*> actorList;
	Player* player;
	Level* curLvl;
	int playerScore;
	void setDisplayText();
};

#endif // STUDENTWORLD_H_
