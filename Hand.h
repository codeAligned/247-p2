#ifndef _HAND_
#define _HAND_

#include <vector>
#include "Card.h"

class Hand {
public:
    ~Hand();
    Hand(Hand&);
    Hand(std::vector<Card*>);
    void play(Card);
    void discard(Card);
    std::vector<Card*> getDiscards();
    std::vector<Card*> getCards();
private:
    std::vector<Card*> cards_;
    std::vector<Card*> discards_;
};

#endif