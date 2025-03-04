#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imgID, int startX, int startY, StudentWorld* inpWorld, int dir = 0);
	virtual void doSomething() = 0;
	virtual void getAttacked() = 0;
	virtual void burpAttack() = 0;
	virtual ~Actor();
	void kill(); // Queue Actor for removal
	void incTick(int ticks);
	int getTicks() const;
	bool isDead() const;
protected:
	StudentWorld* getWorld() const;
private:
	StudentWorld* curWorld;
	bool dead;
	int tickCount;
};


class Floor : public Actor {
public:
	Floor(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	void getAttacked();
	void burpAttack();
private:
};




class Player : public Actor {
public:
	Player(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	int getBurps() const;
	void changeBurps(int num);
	void getAttacked();
	void burpAttack();
	void freeze();
private:
	int burps;
	int jumpSeq;
	int jumpDir;
	int freezeTimer;
};

class Ladder : public Actor {
public:
	Ladder(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	void getAttacked();
	void burpAttack();
};


class Kong : public Actor {
public:
	Kong(int startX, int startY, StudentWorld* inpWorld, int dir);
	void doSomething();
	void getAttacked();
	void burpAttack();
private:
	bool flee;
	int barrelTick;
};

class Barrel : public Actor {
public:
	Barrel(int startX, int startY, StudentWorld* inpWorld, int dir);
	void doSomething();
	void getAttacked();
	void burpAttack();
private:
	bool falling;
};

class Fireball : public Actor {
public:
	Fireball(int startX, int startY, StudentWorld* inpWorld, int dir);
	void doSomething();
	void getAttacked();
	void burpAttack();
private:
	int climbingState;
};

class Koopa : public Actor {
public:
	Koopa(int startX, int startY, StudentWorld* inpWorld, int dir);
	void doSomething();
	void getAttacked();
	void burpAttack();
private:
	int freezeTimer;
};

class Bonfire : public Actor {
public:
	Bonfire(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	void getAttacked();
	void burpAttack();
};

class ExtraLifeGoodie : public Actor {
public:
	ExtraLifeGoodie(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	void getAttacked();
	void burpAttack();
};

class GarlicGoodie : public Actor {
public:
	GarlicGoodie(int startX, int startY, StudentWorld* inpWorld);
	void doSomething();
	void getAttacked();
	void burpAttack();
};

class Burp : public Actor {
public:
	Burp(int startX, int startY, StudentWorld* inpWorld, int dir);
	void doSomething();
	void getAttacked();
	void burpAttack();
};

// Classes needed: Kong, Barrels, Fireballs, Koopas, Bonfire, Ladders, Extra Life Goodies, Garlic Goodies, Burps(from Player)




#endif // ACTOR_H_
