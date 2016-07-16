/*  Created by Jo Atlee on 06/07/09. */

#include "RoundModel.h"
using namespace std;

RoundModel::RoundModel() : topCard_(-1) {}

RoundModel::RoundModel(std::vector<Player*> players, Deck* new_deck) {
    deck_ = new_deck;
    vector<Hand*> hands = new_deck->dealCards();
    for(int i = 0; i < hands.size(); ++i) {
        players.at(i)->setHand(hands.at(i));
    }
    players_ = players;
}

RoundModel::~RoundModel() {
    delete deck_;
    for(int i= 0; i < played_cards_.size(); ++i) {
        delete played_cards_.at(i);
    }
}

vector<Player*> RoundModel::getPlayers() const{
    return players_;
}

Player* RoundModel::getPlayer(int playerID) const{
    return players_.at(playerID-1);
}

Deck* RoundModel::getDeck() const{
    return deck_;
}

// Add card c to list of played cards in round
void RoundModel::playCard(Card c) {
    //TODO should use copy constructor
    Card* newCard = new Card(c.getSuit(),c.getRank());
    played_cards_.push_back(newCard);
}

vector<Card*> RoundModel::getPlayedCards() const {
    return played_cards_;
}

// Delete player with id playerID and replace with newPlayer
void RoundModel::ragequit(int playerID, Player* newPlayer) {
    --playerID;
    Player* temp = players_.at(playerID);
    players_.at(playerID) = newPlayer;
    delete temp;
}

// Reset played_cards_ to be empty, shuffle deck and deal new cards
void RoundModel::newRound() {
    for(int i= 0; i < played_cards_.size(); ++i) {
        delete played_cards_.at(i);
    }
    played_cards_ = vector<Card*>();

    deck_->shuffle();
    vector<Hand*> hands = deck_->dealCards();

    // Delete old hand and set new hand for player
    for(int i = 0; i < hands.size(); ++i){
        players_.at(i)->deleteHand();
        players_.at(i)->setHand(hands.at(i));
    }
}

Suit RoundModel::suit() {
  if (topCard_ == -1) return NOSUIT;

  return (Suit) (topCard_ % numSuits);
}

Rank RoundModel::face() {
  if (topCard_ == -1) return NOFACE;

  return (Rank) (topCard_ / numSuits);
}

void RoundModel::nextCard() {
  if (topCard_ == numCards-1) return;

  topCard_ += 1;
  notify();

}

void RoundModel::resetCards() {
  topCard_ = -1;
  notify();
}
