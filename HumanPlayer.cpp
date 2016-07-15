#include "HumanPlayer.h"

using namespace std;

HumanPlayer::HumanPlayer() {} // Needed for new human player initialization in Game

bool HumanPlayer::isHuman() const {
    return true;
}

Command HumanPlayer::playTurn(RoundController* controller) {
    // Get command inputted by user
    Command cmd;
    cin.clear();
    cout << ">";
    cin>>cmd;
    return cmd;

}