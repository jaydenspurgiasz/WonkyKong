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
  Level* getCurLevel() const;
  Player* getPlayer() const;
  void kongFlee();
  void addActor(Actor* newAc);
  void attackAll(int x, int y);
  void burpAll(int x, int y);
  ~StudentWorld();
private:
	std::list<Actor*> actorList;
	Player* player;
	Level* curLvl;
	void setDisplayText();
	void removeDeadActors();
	bool curLvlFinished;
};

#endif // STUDENTWORLD_H_
