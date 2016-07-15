#include "ComputerPlayer.h"

using namespace std;

ComputerPlayer::ComputerPlayer() {}

ComputerPlayer::ComputerPlayer(Player& player) {
    hand_ = new Hand(*player.getHand());
    gameScore_ = player.getScore();
}

bool ComputerPlayer::isHuman() const{
    return false;
}

// Automatically plays computer's turn
Command ComputerPlayer::playTurn(RoundController* controller) {
    vector<Card*> legalPlays = controller->calculateLegalPlay(this);
    vector<Card*> hand = hand_->getCards();

    // Check hand for legal plays. If found, play it.
    for(int i = 0; i < hand.size(); ++i) {
        for(int j = 0; j < legalPlays.size(); ++j) {
            if(*hand.at(i) == *legalPlays.at(j)){
                Command cmd = Command();
                cmd.card = *hand.at(i);
                cmd.type = PLAY;
                return cmd;
            }
        }
    }
    //else discard first card in hand
    Command cmd = Command();
    cmd.card = *hand.at(0);
    cmd.type = DISCARD;
    return cmd;
}