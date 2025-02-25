/*
#ifndef PLAYER_H_
#define PLAYER_H_

#include "AliveActor.h"

class StudentWorld;

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

#endif // PLAYER_H_
*/