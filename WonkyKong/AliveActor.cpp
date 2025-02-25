/*
#include "AliveActor.h"
#include "StudentWorld.h"

AliveActor::AliveActor(int imgID, int startX, int startY, StudentWorld* inpWorld, int dir):
	Actor(imgID, startX, startY, dir), dead(false), curWorld(inpWorld)
{
}

bool AliveActor::isDead() const {
	return dead;
}

StudentWorld* AliveActor::getWorld() const {
	return curWorld;
}

void AliveActor::kill() {
	dead = true;
}

AliveActor::~AliveActor() {
	curWorld = nullptr;
}
*/