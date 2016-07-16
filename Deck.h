#ifndef _DECK_
#define _DECK_

#include <iostream>
#include <vector>
#include "Card.h"
#include "Hand.h"

class Deck {
public:
    Deck();
    Deck(int);
    ~Deck();
    void shuffle();
    std::vector<Hand*> dealCards();
    Card* at(int);
private:
    std::vector<Card*> cards_;
    const int CARD_COUNT = 52;
};

#endif