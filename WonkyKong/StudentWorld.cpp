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
: GameWorld(assetPath), actorList(), playerScore(0),  curLvl(nullptr)
{
}

Level* StudentWorld::getLevel() const {
    return curLvl;
}

int StudentWorld::init()
{
    int lvlNum = 0; // How to get current level??????
    std::ostringstream lvlInfo;
    lvlInfo << "level" << std::setw(2) << std::setfill('0') << lvlNum << ".txt";
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
            switch (item) {
                case Level::player:
                    player = new Player(x, y, this);
                    break;
                case Level::floor:
                    Actor* fl = new Floor(x, y);
                    actorList.push_back(fl); // Creates a new Floor actor at position: x, y
                    break;
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");

    // Make Player doSomething
    player->doSomething();
    // If do Something causes player to die
    if (false) {
        return GWSTATUS_PLAYER_DIED;
    }

    // Make all actors doSomething
    for (std::list<Actor*>::iterator p = actorList.begin(); p != actorList.end(); p++) {
        (*p)->doSomething();

        // If Kong is fleeing
        if (false) {
            playerScore++; // Add by 1000? Double check spec !!!!!!!!!
            return GWSTATUS_FINISHED_LEVEL;
        }

        // If other actor needs to be deleted
        if (false) {
            // Figure out implementation after
        }
    }

    std::ostringstream scoreInfo;
    scoreInfo << "Score: " << std::setw(7) << std::setfill('0') << playerScore << std::endl;
    int lvlNum = 0; // How to get current lvl ???????
    std::ostringstream lvlInfo;
    lvlInfo << "\tLevel: " << std::setw(2) << std::setfill('0') << lvlNum << std::endl;
    std::ostringstream livesInfo;
    livesInfo << "\tLives: " << std::setw(2) << std::setfill('0') << player->getLives() << std::endl;
    std::ostringstream burpsInfo;
    burpsInfo << "\tBurps: " << std::setw(2) << std::setfill('0') << player->getBurps() << std::endl;
    std::string info = scoreInfo.str() + lvlInfo.str() + livesInfo.str() + burpsInfo.str();
    setGameStatText(info);
    
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
