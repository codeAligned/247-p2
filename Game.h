#ifndef _GAME_
#define _GAME_

#include <iostream>
#include "RoundController.h"

class Game {
public:
    Game(int seed = 0);
    ~Game();
    void displayWinner(); // Displays winner if game is over.
    bool gameOver(); // Checks if game is over
private:
    void createPlayers(); // Creates 4 players at start of game.
    static const int NUM_PLAYERS=4;
    RoundController* round_controller_;
    std::vector<Player*> players_;
};

#endif
