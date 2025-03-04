#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"
#include <list>
#include <cmath> // For pow() and sqrt()
#include <algorithm> // For max()

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imgID, int startX, int startY, StudentWorld* inpWorld, int dir):
	GraphObject(imgID, startX, startY, dir, 1.0), curWorld(inpWorld), dead(false), tickCount(0) {
}

bool Actor::isDead() const {
	return dead;
}

StudentWorld* Actor::getWorld() const {
	return curWorld;
}

void Actor::kill() {
	dead = true;
}

int Actor::getTicks() const {
	return tickCount;
}

void Actor::incTick(int ticks) {
	tickCount += ticks;
}

Actor::~Actor() {}


// Floor
Floor::Floor(int startX, int startY, StudentWorld* inpWorld) :
	Actor(IID_FLOOR, startX, startY, inpWorld, none)
{
}

void Floor::doSomething() {
	// Floor may not have to do anything
};

void Floor::getAttacked() {
	// Nothing
}

void Floor::burpAttack() {
	// Nothing
}



// Player
Player::Player(int startX, int startY, StudentWorld* inpWorld) :
	Actor(IID_PLAYER, startX, startY, inpWorld, right), burps(0), jumpSeq(0), jumpDir(0), freezeTimer(0)
{
}

void Player::doSomething() {
	// If player dead, return
	if (isDead()) {
		return;
	}

	Level* curLvl = getWorld()->getCurLevel();
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
	if (freezeTimer > 0) {
		freezeTimer--;
		return;
	}

	// Gravity logic
	if (curLvl->getContentsOf(getX(), getY() - 1) != Level::floor 
		&& curLvl->getContentsOf(getX(), getY() - 1) != Level::ladder 
		&& curLvl->getContentsOf(getX(), getY()) != Level::ladder) { // Is there anything under the Player
		moveTo(getX(), getY() - 1);
		return;
	}

	// Check for input
	int inpCh;
	if (getWorld()->getKey(inpCh)) {
		switch (inpCh) {
		case KEY_PRESS_LEFT:
			if (getDirection() != GraphObject::left) {
				setDirection(left);
			}
			else {
				if (curLvl->getContentsOf(getX() - 1, getY()) != Level::floor) {
					moveTo(getX() - 1, getY());
				}
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != GraphObject::right) {
				setDirection(right);
			}
			else {
				if (curLvl->getContentsOf(getX() + 1, getY()) != Level::floor) {
					moveTo(getX() + 1, getY());
				}
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
				if (curLvl->getContentsOf(getX(), getY()+1) != Level::floor
					&& (curLvl->getContentsOf(getX(), getY()) == Level::ladder ||
					curLvl->getContentsOf(getX(), getY() - 1) == Level::ladder ||
					curLvl->getContentsOf(getX(), getY() - 1) == Level::floor)) {
					jumpSeq++;
					jumpDir = getDirection();
					moveTo(getX(), getY() + 1); // Start of Jump
					getWorld()->playSound(SOUND_JUMP); // Jump sound effect
					return;
				}
			}
			break;
		case KEY_PRESS_TAB:
			//BURP
			if (burps > 0) {
				getWorld()->playSound(SOUND_BURP);
				int burpX, burpY = 0;
				getPositionInThisDirection(getDirection(), 1, burpX, burpY);
				getWorld()->addActor(new Burp(burpX, burpY, getWorld(), getDirection()));
				changeBurps(-1);
			}
			break;
		}
	}
}

int Player::getBurps() const {
	return burps;
}

void Player::changeBurps(int num) {
	burps += num;
}

void Player::freeze() {
	freezeTimer = 50;
}

void Player::getAttacked() {
	getWorld()->playSound(SOUND_PLAYER_DIE);
	kill();
}

void Player::burpAttack() {

}

// Ladder
Ladder::Ladder(int startX, int startY, StudentWorld* inpWorld): Actor(IID_LADDER, startX, startY, inpWorld, none) {}

void Ladder::doSomething() {}

void Ladder::getAttacked() {

}

void Ladder::burpAttack() {

}



//Kong
Kong::Kong(int startX, int startY, StudentWorld* inpWorld, int dir) : Actor(IID_KONG, startX, startY, inpWorld, dir), flee(false) {
	barrelTick = std::max(200 - 50 * getWorld()->getLevel(), 50);
}

void Kong::doSomething() {
	if (isDead()) {
		return;
	}
	
	increaseAnimationNumber();

	Player* pl = getWorld()->getPlayer(); // If euclidian distance < 2
	if (std::sqrt(std::pow(pl->getX() - getX(), 2.0) + std::pow(pl->getY() - getY(), 2.0)) < 2.0) {
		flee = true;
	}

	if (!flee && barrelTick <= 0) {
		// Throw a barrel
		int nextX, nextY = 0;
		getPositionInThisDirection(getDirection(), 1, nextX, nextY);
		getWorld()->addActor(new Barrel(nextX, nextY, getWorld(), getDirection()));
		barrelTick = std::max(200 - 50 * getWorld()->getLevel(), 50);
	}
	else {
		barrelTick--;
	}
	// Every 5th call to doSomething()
	if (getTicks() == 4) { // Check barrelTick % 5 == 0 ?
		incTick(-1 * getTicks());
		if (flee) {
			moveTo(getX(), getY()+1);
			if (getY() > 19) {
				getWorld()->increaseScore(1000);
				getWorld()->playSound(SOUND_FINISHED_LEVEL);
				getWorld()->kongFlee();
			}
		}
	}
	else {
		incTick(1);
	}
}

void Kong::getAttacked() {

}

void Kong::burpAttack() {

}


// Barrels
Barrel::Barrel(int startX, int startY, StudentWorld* inpWorld, int dir) : Actor(IID_BARREL, startX, startY, inpWorld, dir), falling(false) {}

void Barrel::doSomething() {
	if (isDead()) {
		return;
	}

	Player* pl = getWorld()->getPlayer();
	if (pl->getX() == getX() && pl->getY() == getY()) {
		pl->getAttacked();
		return;
	}

	Level* lvl = getWorld()->getCurLevel();
	if (lvl->getContentsOf(getX(), getY()-1) != Level::floor) {
		moveTo(getX(), getY()-1);
		falling = true;
		return;
	}
	if (lvl->getContentsOf(getX(), getY()-1) == Level::floor && falling) {
		falling = false;
		if (getDirection() == GraphObject::left) {
			setDirection(GraphObject::right);
		}
		else {
			setDirection(GraphObject::left);
		}
	}

	if (getTicks() == 9) {
		incTick(-1*getTicks());
		int nextX, nextY = 0;
		getPositionInThisDirection(getDirection(), 1, nextX, nextY);
		if (lvl->getContentsOf(nextX, nextY) == Level::floor) {
			if (getDirection() == GraphObject::left) {
				setDirection(GraphObject::right);
			}
			else {
				setDirection(GraphObject::left);
			}
		}
		else {
			moveTo(nextX, nextY);
		}

		if (pl->getX() == getX() && pl->getY() == getY()) {
			pl->getAttacked();
			return;
		}
	}
	else {
		incTick(1);
	}
}

void Barrel::getAttacked() {
	// Attacked by Bonfire
	kill();
}

void Barrel::burpAttack() {
	kill(); // Handle attack from Burp
	getWorld()->playSound(SOUND_ENEMY_DIE);
	getWorld()->increaseScore(100);
}


// Fireballs
Fireball::Fireball(int startX, int startY, StudentWorld* inpWorld, int dir) : Actor(IID_FIREBALL, startX, startY, inpWorld, dir), climbingState(none) {}

void Fireball::doSomething() {
	if (isDead()) {
		return;
	}

	Player* pl = getWorld()->getPlayer();
	if (pl->getX() == getX() && pl->getY() == getY()) { // Attack player if on same square
		pl->getAttacked();
	}

	if (getTicks() == 9) { // Every tenth call
		incTick(-1 * getTicks());

		// Handle climb movement
		Level* lvl = getWorld()->getCurLevel();
		if (lvl->getContentsOf(getX(), getY()) == Level::ladder
			&& lvl->getContentsOf(getX(), getY()+1) != Level::floor
			&& climbingState != GraphObject::down) {
			int randNum = randInt(1, 3);
			if (climbingState == GraphObject::up || randNum == 1) {
				climbingState = GraphObject::up;
				moveTo(getX(), getY()+1);
				// Step e.
				if (pl->getX() == getX() && pl->getY() == getY()) { // Attack player if on same square
					pl->getAttacked();
				}
				return;
			}
		}
		if (lvl->getContentsOf(getX(), getY()-1) == Level::ladder
			&& climbingState != GraphObject::up) {
			int randNum = randInt(1, 3);
			if (climbingState == GraphObject::down || randNum == 1) {
				climbingState = GraphObject::down;
				moveTo(getX(), getY()-1);
				// Step e.
				if (pl->getX() == getX() && pl->getY() == getY()) { // Attack player if on same square
					pl->getAttacked();
				}
				return;
			}
		} // Check if climbing
		if (climbingState != GraphObject::none) {
			int nextX, nextY = 0;
			getPositionInThisDirection(climbingState, 1, nextX, nextY);
			if (lvl->getContentsOf(nextX, nextY) == Level::floor
				|| lvl->getContentsOf(nextX, nextY) != Level::ladder) {
				climbingState = GraphObject::none;
			}
		}
		int nextX, nextY = 0; // Move horizontally
		getPositionInThisDirection(getDirection(), 1, nextX, nextY);
		if (lvl->getContentsOf(nextX, nextY) == Level::floor
			|| (lvl->getContentsOf(nextX, nextY - 1) != Level::floor
			&& lvl->getContentsOf(nextX, nextY - 1) != Level::ladder)) {
			if (getDirection() == GraphObject::left) {
				setDirection(GraphObject::right);
			}
			else {
				setDirection(GraphObject::left);
			}
		} else {
			moveTo(nextX, nextY);
		}
	}
	else {
		incTick(1);
	}

}

void Fireball::getAttacked() {
	// Nothing
}

void Fireball::burpAttack() {
	kill(); // What happens when a Fireball is burped on
	getWorld()->playSound(SOUND_ENEMY_DIE);
	getWorld()->increaseScore(100);
	int randNum = randInt(1, 3);
	if (randNum == 3) { // Lucky number 3
		getWorld()->addActor(new GarlicGoodie(getX(), getY(), getWorld()));
	}
}


// Koopas
Koopa::Koopa(int startX, int startY, StudentWorld* inpWorld, int dir) : Actor(IID_KOOPA, startX, startY, inpWorld, dir), freezeTimer(0) {}

void Koopa::doSomething() {
	if (isDead()) {
		return;
	}
	Player* pl = getWorld()->getPlayer();
	if (pl->getX() == getX() && pl->getY() == getY() && freezeTimer == 0) {
		pl->freeze(); // Freeze player
		freezeTimer = 50;
		return;
	}
	if (freezeTimer > 0) { // Track freeze
		freezeTimer--;
	}
	if (getTicks() == 9) {
		incTick(-1 * getTicks());
		Level* lvl = getWorld()->getCurLevel();
		int nextX, nextY = 0; // Move horizontally
		getPositionInThisDirection(getDirection(), 1, nextX, nextY);
		if (lvl->getContentsOf(nextX, nextY) == Level::floor
			|| (lvl->getContentsOf(nextX, nextY - 1) != Level::floor
				&& lvl->getContentsOf(nextX, nextY - 1) != Level::ladder)) {
			if (getDirection() == GraphObject::left) {
				setDirection(GraphObject::right);
			}
			else {
				setDirection(GraphObject::left);
			}

			// Check to freeze Player
			if (pl->getX() == getX() && pl->getY() == getY() && freezeTimer == 0) {
				pl->freeze(); // Freeze player
				freezeTimer = 50;
				return;
			}
		}
		else {
			moveTo(nextX, nextY);
		}
	}
	else {
		incTick(1);
	}
}

void Koopa::getAttacked() {
	// Nothing happens
}

void Koopa::burpAttack() {
	kill(); // If Koopa gets burped on
	getWorld()->playSound(SOUND_ENEMY_DIE);
	getWorld()->increaseScore(100);
	int randNum = randInt(1, 3);
	if (randNum == 3) {
		getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
	}
}


// Bonfire
Bonfire::Bonfire(int startX, int startY, StudentWorld* inpWorld) : Actor(IID_BONFIRE, startX, startY, inpWorld, none) {}

void Bonfire::doSomething() {
	increaseAnimationNumber();

	Player* pl = getWorld()->getPlayer();
	if (getX() == pl->getX() && getY() == pl->getY()) {
		pl->getAttacked();
	}

	getWorld()->attackAll(getX(), getY());
}

void Bonfire::getAttacked() {

}

void Bonfire::burpAttack() {

}


// ExtraLifeGoodies
ExtraLifeGoodie::ExtraLifeGoodie(int startX, int startY, StudentWorld* inpWorld) : Actor(IID_EXTRA_LIFE_GOODIE, startX, startY, inpWorld, none) {}

void ExtraLifeGoodie::doSomething() {
	if (isDead()) {
		return;
	}

	Player* pl = getWorld()->getPlayer();
	if (pl->getX() == getX() && pl->getY() == getY()) {
		getWorld()->incLives();
		getWorld()->increaseScore(50);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		kill();
	}
}

void ExtraLifeGoodie::getAttacked() {
	
}

void ExtraLifeGoodie::burpAttack() {

}


// GarlicGoodies
GarlicGoodie::GarlicGoodie(int startX, int startY, StudentWorld* inpWorld) : Actor(IID_GARLIC_GOODIE, startX, startY, inpWorld, none) {}

void GarlicGoodie::doSomething() {
	if (isDead()) {
		return;
	}

	Player* pl = getWorld()->getPlayer();
	if (pl->getX() == getX() && pl->getY() == getY()) {
		pl->changeBurps(5);
		getWorld()->increaseScore(25);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		kill();
	}
}

void GarlicGoodie::getAttacked() {

}

void GarlicGoodie::burpAttack() {

}



// Burps
Burp::Burp(int startX, int startY, StudentWorld* inpWorld, int dir) : Actor(IID_BURP, startX, startY, inpWorld, dir) {}

void Burp::doSomething() {
	if (isDead()) {
		return;
	}

	incTick(1);
	if (getTicks() >= 5) {
		kill(); // Remove burp after num of ticks
	}

	getWorld()->burpAll(getX(), getY());

}

void Burp::getAttacked() {

}

void Burp::burpAttack() {

}