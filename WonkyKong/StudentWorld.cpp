#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>

/*
#include "Player.h"
#include "Floor.h"
*/

#include "Actor.h"

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), actorList(), player(nullptr), curLvl(nullptr), curLvlFinished(false)
{
}

Level* StudentWorld::getCurLevel() const {
    return curLvl;
}

Player* StudentWorld::getPlayer() const {
    return player;
}

void StudentWorld::addActor(Actor* newAc) {
    actorList.push_back(newAc);
}

void StudentWorld::kongFlee() {
    curLvlFinished = true;
}

void StudentWorld::attackAll(int x, int y) {
    for (std::list<Actor*>::iterator p = actorList.begin(); p != actorList.end(); p++) {
        if ((*p)->getX() == x && (*p)->getY() == y) {
            (*p)->getAttacked(); // Only a barrel would be attacked by this
        }
    }
}

void StudentWorld::burpAll(int x, int y) {
    for (std::list<Actor*>::iterator p = actorList.begin(); p != actorList.end(); p++) {
        if ((*p)->getX() == x && (*p)->getY() == y) {
            (*p)->burpAttack();
        }
    }
}

int StudentWorld::init()
{
    std::ostringstream lvlInfo;
    lvlInfo << "level" << std::setw(2) << std::setfill('0') << getLevel() << ".txt";
    std::string lvlString = lvlInfo.str();
    curLvl = new Level(assetPath());
    Level::LoadResult res = curLvl->loadLevel(lvlString);
    if (res == Level::load_fail_bad_format || res == Level::load_fail_file_not_found) {
        std::cout << "Loading the level was unsuccessful" << std::endl;
        return -1;
    }

    
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
            Level::MazeEntry item = curLvl->getContentsOf(x, y);
            Actor* ac = nullptr;
            int randDir = GraphObject::left;
            switch (item) {
                case Level::player:
                    player = new Player(x, y, this);
                    break;
                case Level::floor:
                    ac = new Floor(x, y, this);
                    break;
                case Level::ladder:
                    ac = new Ladder(x, y, this);
                    break;
                case Level::left_kong:
                    ac = new Kong(x, y, this, GraphObject::left);
                    break;
                case Level::right_kong:
                    ac = new Kong(x, y, this, GraphObject::right);
                    break;
                case Level::fireball:
                    if (randInt(0, 1) == 1) {
                        randDir = GraphObject::right;
                    }
                    ac = new Fireball(x, y, this, randDir);
                    break;
                case Level::koopa:
                    if (randInt(0, 1) == 1) {
                        randDir = GraphObject::right;
                    }
                    ac = new Koopa(x, y, this, randDir);
                    break;
                case Level::bonfire:
                    ac = new Bonfire(x, y, this);
                    break;
                case Level::extra_life:
                    ac = new ExtraLifeGoodie(x, y, this);
                    break;
                case Level::garlic:
                    ac = new GarlicGoodie(x, y, this);
                    break;
            }
            if (ac != nullptr)
                actorList.push_back(ac); // Creates a new Actor at position: x, y
        }
    }
    curLvlFinished = false;

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    /*
    // This code is here merely to allow the game to build, run, and terminate after you type q
    setGameStatText("Game will end when you type q");
    */

    // Make Player doSomething
    if (!player->isDead()) {
        player->doSomething();
    }

    // Make all actors doSomething
    for (std::list<Actor*>::iterator p = actorList.begin(); p != actorList.end(); p++) {
        if (!(*p)->isDead()) {
            (*p)->doSomething();
        }

        // If Kong is fleeing
        if (curLvlFinished) {
            return GWSTATUS_FINISHED_LEVEL; 
        }
    }

    if (player->isDead()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    removeDeadActors();

    setDisplayText();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // Delete everything in actorList
    for (std::list<Actor*>::iterator p = actorList.begin(); p != actorList.end(); p++) {
        delete (*p);
        *p = nullptr;
    }
    actorList.erase(actorList.begin(), actorList.end());

    // Delete curLvl
    delete curLvl;
    curLvl = nullptr;

    // Delete player
    delete player;
    player = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::setDisplayText() {
    std::ostringstream scoreInfo;
    scoreInfo << "Score: " << std::setw(7) << std::setfill('0') << getScore() << "  ";
    std::ostringstream lvlInfo;
    lvlInfo << "\tLevel: " << std::setw(2) << std::setfill('0') << getLevel() << "  ";
    std::ostringstream livesInfo;
    livesInfo << "\tLives: " << std::setw(2) << std::setfill('0') << getLives() << "  ";
    std::ostringstream burpsInfo;
    burpsInfo << "\tBurps: " << std::setw(2) << std::setfill('0') << player->getBurps() << "  ";
    std::string info = scoreInfo.str() + lvlInfo.str() + livesInfo.str() + burpsInfo.str();
    
    setGameStatText(info);
}

void StudentWorld::removeDeadActors() {
    std::list<Actor*>::iterator i = actorList.begin();
    while (i != actorList.end()) {
        if ((*i)->isDead()) {
            delete (*i);
            i = actorList.erase(i);
        }
        else {
            i++;
        }
    }
}