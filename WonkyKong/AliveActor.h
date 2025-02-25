/*
#ifndef ALIVEACTOR_H_
#define ALIVEACTOR_H_

#include "Actor.h"

class StudentWorld;

class AliveActor : public Actor {
public:
	AliveActor(int imgID, int startX, int startY, StudentWorld* inpWorld, int dir = 0);
	virtual void doSomething() = 0;
	bool isDead() const;
	void kill();
	StudentWorld* getWorld() const;
	~AliveActor();
private:
	bool dead;
	StudentWorld* curWorld;
};



#endif // ALIVEACTOR_H_
*/