/*
#include "Player.h"
#include "Level.h"
#include "StudentWorld.h"

Player::Player(int startX, int startY, StudentWorld* inpWorld):
	AliveActor(IID_PLAYER, startX, startY, inpWorld, right), burps(3), lives(0), jumpSeq(0)
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
		if (jumpSeq < 4) { // If jumpSeq = 1, 2, 3
			int nextX = getX();
			int nextY = getY();
			getPositionInThisDirection(getDirection(), 1, nextX, nextY);
			if (curLvl->getContentsOf(nextX, nextY) == Level::floor ||
				curLvl->getContentsOf(getX(), getY()) == Level::ladder) {
				jumpSeq = 0;
			}
			else {
				if (getDirection() == right || getDirection() == left) {
					if (curLvl->getContentsOf(nextX, nextY) == Level::floor) {
						jumpSeq = 0;
					}
					moveTo(nextX, nextY);
				}
				jumpSeq++;
				return;
			}
		}
		else {
			jumpSeq = 0;
		}
	}

	// Frozen logic

	// Gravity logic
	if (curLvl->getContentsOf(getX(), getY()-1) == Level::empty) { // Is there anything under the player
		moveTo(getX(), getY()-1);
		return;
	}

	// Check for input
	int inpCh;
	if (getWorld()->getKey(inpCh)) {
		switch (inpCh) {
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (curLvl->getContentsOf(getX()-1, getY()) != Level::floor) {
				moveTo(getX()-1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (curLvl->getContentsOf(getX() + 1, getY()) != Level::floor) {
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_UP:
			// Implement after ladder functionality
			break;
		case KEY_PRESS_DOWN:
			// Implement after ladder functionality
			break;
		case KEY_PRESS_SPACE:
			if (jumpSeq == 0) {
				if (curLvl->getContentsOf(getX(), getY()) == Level::ladder ||
					curLvl->getContentsOf(getX(), getY() - 1) == Level::ladder ||
					curLvl->getContentsOf(getX(), getY() - 1) == Level::floor) {
					jumpSeq++;
					moveTo(getX(), getY()+1);
					getWorld()->playSound(SOUND_JUMP);
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
*/