/*  Created by Jo Atlee on 06/07/09. */
#ifndef ROUND_MODEL_H
#define ROUND_MODEL_H

#include <vector>
#include "subject.h"
#include "Card.h"
#include "Player.h"
#include "Deck.h"

const int numFaces = 6;
const int numSuits = 4;
const int numCards = numFaces * numSuits;

class RoundModel : public Subject {
public:
    RoundModel(int);
    RoundModel(std::vector<Player*>, Deck*);
    ~RoundModel();
    std::vector<Player*> getPlayers() const;
    Player* getPlayer(int playerID) const;
    Deck* getDeck() const;
    void playCard(Card c);
    std::vector<Card*> getPlayedCards() const;
    void ragequit(int, Player*);
    void newRound();
    Suit suit();
    Rank face();
    void nextCard();
    void resetCards();
    void setPlayers(bool[4]);
    void notifyView();
private:
    std::vector<Player*> players_;
    std::vector<Card*> played_cards_;
    Deck* deck_;
    int topCard_;
}; // Model


#endif
