#include "Game.h"

using namespace std;

// Create players and round controller.
Game::Game(int seed) {
    createPlayers();
    round_controller_ = nullptr;
    while (!gameOver()) {
        if (round_controller_) {
            round_controller_->newRound();
            int player_7spades = round_controller_->who7Spades();
            round_controller_->startRound(player_7spades);
        }
        else {
            round_controller_ = new RoundController(players_, seed);
        }
    }
    displayWinner();
}

void Game::createPlayers() {
    for (int i = 1; i <= NUM_PLAYERS; ++i) {
        char playerType;
        cout << "Is player " << i << " a human(h) or a computer(c)?" << endl << ">";
        cin >> playerType;
        if (playerType == 'c') {
            players_.push_back( new ComputerPlayer() );
        }
        else if (playerType == 'h'){
            players_.push_back( new HumanPlayer() );
        }
        else {
            throw "Response not c or h";
        }
    }
}

Game::~Game() {
    for(int i= 0; i < players_.size(); ++i){
        delete players_.at(i);
    }
    delete round_controller_;
}

//iterate over players, check score over 80 for any. If so, game over.
bool Game::gameOver() {
    if(round_controller_) {
        players_ = round_controller_->getPlayers();
    }
    for(int i= 0; i < players_.size(); ++i) {
        if (players_.at(i)->getScore() >= 80) {
            return true;
        }
    }
    return false;
}

void Game::displayWinner() {
    int winner = 0;
    int min_score = players_.at(0)->getScore();
    for (int i = 0; i < players_.size(); ++i) {
        if (players_.at(i)->getScore() < min_score) {
            min_score = players_.at(i)->getScore();
            winner = i;
        }
    }
    cout << "Player " << winner+1 << " wins!" << endl;
}