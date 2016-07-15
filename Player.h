#ifndef _PLAYER_
#define _PLAYER_

#include <iostream>
#include <vector>
#include "Card.h"
#include "Hand.h"
#include "Command.h"

class RoundController;

class Player {
public:
    Player();
    Player(Hand*);
   ~Player();
    virtual bool isHuman() const = 0;
    void deleteHand();
    std::vector<Card*> getDiscards();
    std::vector<Card*> getCards();
    void discardCard(Card);
    void playCard(Card);
    void setHand(Hand*);
    void setScore(int);
    Hand* getHand() const;
    int getScore() const;
    virtual Command playTurn(RoundController*);
protected:
    Hand* hand_;
    int gameScore_;
};


#endif