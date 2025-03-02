#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imgID, int startX, int startY, int dir): 
	GraphObject(imgID, startX, startY, dir, 1.0) {
}



AliveActor::AliveActor(int imgID, int startX, int startY, StudentWorld* inpWorld, int dir) :
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





Floor::Floor(int startX, int startY) :
	Actor(IID_FLOOR, startX, startY, none)
{
}

void Floor::doSomething() {
	// Floor may not have to do anything
};




Player::Player(int startX, int startY, StudentWorld* inpWorld) :
	AliveActor(IID_PLAYER, startX, startY, inpWorld, right), burps(0), lives(0), jumpSeq(0), jumpDir(0)
{
}

void Player::doSomething() {
	// If player dead, return
	if (isDead()) {
		return;
	}

	Level* curLvl = getWorld()->getLevel();
	// Jump logic
	if (jumpSeq > 0) {
		// jumpSeq=0: Move up, jumpSeq=1,2,3: Move in dir, jumpSeq = 4: Move down
		if (jumpSeq > 4) {
			jumpSeq = 0;
			return;
		}
		if (jumpSeq == 4) {
			if (curLvl->getContentsOf(getX(), getY()-1) == Level::floor) {
				jumpSeq = 0;
				return;
			}
			moveTo(getX(), getY()-1);
		}
		else if (jumpDir == left || jumpDir == right) {
			int nextX, nextY = 0;
			getPositionInThisDirection(jumpDir, 1, nextX, nextY);
			if (curLvl->getContentsOf(nextX, nextY) == Level::floor ||
				nextX < 0 || nextY < 0 || nextX > 19 || nextY > 19) {
				jumpSeq = 0;
				return;
			}
			moveTo(nextX, nextY);
		}
		jumpSeq++;
		if (curLvl->getContentsOf(getX(), getY()) == Level::ladder)
			jumpSeq = 0;
		return;
	}

	// Frozen logic

	// Gravity logic
	if (curLvl->getContentsOf(getX(), getY() - 1) != Level::floor && curLvl->getContentsOf(getX(), getY() - 1) != Level::ladder) { // Is there anything under the Player
		moveTo(getX(), getY() - 1);
		return;
	}

	// Check for input
	int inpCh;
	if (getWorld()->getKey(inpCh)) {
		switch (inpCh) {
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (curLvl->getContentsOf(getX() - 1, getY()) != Level::floor) {
				moveTo(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (curLvl->getContentsOf(getX() + 1, getY()) != Level::floor) {
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (curLvl->getContentsOf(getX(), getY()) == Level::ladder
				&& curLvl->getContentsOf(getX(), getY() + 1) != Level::floor) {
				moveTo(getX(), getY() + 1);
			}
			break;
		case KEY_PRESS_DOWN:
			if (curLvl->getContentsOf(getX(), getY()-1) == Level::ladder || 
				(curLvl->getContentsOf(getX(), getY()) == Level::ladder
				&& curLvl->getContentsOf(getX(), getY() - 1) != Level::floor)) {
				moveTo(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_SPACE:
			if (jumpSeq == 0) {
				if (curLvl->getContentsOf(getX(), getY()) == Level::ladder ||
					curLvl->getContentsOf(getX(), getY() - 1) == Level::ladder ||
					curLvl->getContentsOf(getX(), getY() - 1) == Level::floor) {
					jumpSeq++;
					jumpDir = getDirection();
					moveTo(getX(), getY() + 1); // Start of Jump
					getWorld()->playSound(SOUND_JUMP); // Jump sound effect
					return;
				}
			}
			break;
		case KEY_PRESS_TAB:
			break;
		}
	}
}

int Player::getLives() const {
	return lives;
}

int Player::getBurps() const {
	return burps;
}


// Ladder
Ladder::Ladder(int startX, int startY): Actor(IID_LADDER, startX, startY, none) {}

void Ladder::doSomething() {}




//Kong
Kong::Kong(int startX, int startY, int dir) : Actor(IID_KONG, startX, startY, dir) {}

void Kong::doSomething() {}



// Barrels
Barrel::Barrel(int startX, int startY) : Actor(IID_BARREL, startX, startY, none) {}

void Barrel::doSomething() {}



// Fireballs
Fireball::Fireball(int startX, int startY) : Actor(IID_FIREBALL, startX, startY, none) {}

void Fireball::doSomething() {}



// Koopas
Koopa::Koopa(int startX, int startY) : Actor(IID_KOOPA, startX, startY, none) {}

void Koopa::doSomething() {}



// Bonfire
Bonfire::Bonfire(int startX, int startY) : Actor(IID_BONFIRE, startX, startY, none) {}

void Bonfire::doSomething() {}



// ExtraLifeGoodies
ExtraLifeGoodie::ExtraLifeGoodie(int startX, int startY) : Actor(IID_EXTRA_LIFE_GOODIE, startX, startY, none) {}

void ExtraLifeGoodie::doSomething() {}



// GarlicGoodies
GarlicGoodie::GarlicGoodie(int startX, int startY) : Actor(IID_GARLIC_GOODIE, startX, startY, none) {}

void GarlicGoodie::doSomething() {}



// Burps
Burp::Burp(int startX, int startY) : Actor(IID_BURP, startX, startY, none) {}

void Burp::doSomething() {}
