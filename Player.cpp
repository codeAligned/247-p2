#include "Player.h"
using namespace std;

Player::Player() {
    gameScore_ = 0;
}

Player::Player(Hand* h) : Player() {
    hand_ = h;
}

Player::~Player() {
    delete hand_;
}

void Player::playCard(Card c){
    hand_->play(c);
}

void Player::discardCard(Card c){
    hand_->discard(c);
}

vector<Card*> Player::getDiscards() {
    return hand_->getDiscards();
}

vector<Card*> Player::getCards() {
    return hand_->getCards();
}

int Player::getScore() const{
    return gameScore_;
}

void Player::setScore(int score) {
    gameScore_ = score;
}

void Player::setHand(Hand* hand){
    hand_ = hand;
}

Hand* Player::getHand() const{
    return hand_;
}

void Player::deleteHand() {
    delete hand_;
}

Command Player::playTurn(RoundController* controller){
    throw "Parent has no specified behaviour";
}