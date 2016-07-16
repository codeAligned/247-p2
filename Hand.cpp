#include "Hand.h"
#include <iostream>
using namespace std;

// Copy constructor for ragequit
Hand::Hand(Hand& hand){
    cards_ = hand.cards_;
    discards_ = hand.discards_;
}

// Overloaded constructor
Hand::Hand(vector<Card*> h){
    cards_ = h;
}

Hand::~Hand() {} // Needed for player's DeleteHand method and destructor to delete hand pointer

// Finds index of card in list of cards.
int findLocation(Card c, vector<Card*> list){
    for(int i = 0; i < list.size(); ++i){
        if( *(list.at(i)) == c ) {
            return i;
        }
    }
    throw "Card not found";
}

void Hand::play(Card c) {
    int loc = findLocation(c,cards_);
    cards_.erase(cards_.begin() + loc);
}

// Remove from hand, add to discards
void Hand::discard(Card c) {
    int loc = findLocation(c,cards_);
    discards_.push_back(cards_.at(loc));
    cards_.erase(cards_.begin() + loc);
}

vector<Card*> Hand::getDiscards() {
    return discards_;
}

vector<Card*> Hand::getCards() {
    return cards_;
}