#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
	Actor(int imgID, int startX, int startY, int dir = 0);
	virtual void doSomething() = 0;
private:
};


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


class Floor : public Actor {
public:
	Floor(int startX, int startY);
	void doSomething();
private:
};




class Player : public AliveActor {
public:
	Player(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	int getLives() const;
	int getBurps() const;
private:
	int burps;
	int lives;
	int jumpSeq;
};

#endif // ACTOR_H_
