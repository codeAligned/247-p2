/*  Created by Jo Atlee on 06/07/09. */

#include "RoundModel.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
using namespace std;

RoundModel::RoundModel(int seed) : topCard_(-1) {
  deck_ = new Deck(seed);
  deck_->shuffle();
}

void RoundModel::updateDeck(int seed) {
    cout << "Updating deck with seed: " << seed << endl;
    delete deck_;
    deck_ = new Deck(seed);
    deck_->shuffle();
}

RoundModel::RoundModel(std::vector<Player*> players, Deck* new_deck) {
    deck_ = new_deck;
    vector<Hand*> hands = new_deck->dealCards();
    for(int i = 0; i < hands.size(); ++i) {
        players.at(i)->setHand(hands.at(i));
    }
    players_ = players;
}

void RoundModel::setPlayers(bool isHuman[4]) {
    for (int i = 1; i <= 4; ++i) {
        if (isHuman[i-1]) {
            Player* newPlayer = new HumanPlayer();
            players_.push_back( newPlayer );
            cout<<"Made player "<<i <<" Human"<<endl;
        }
        else {
            Player* newPlayer = new ComputerPlayer();
            players_.push_back( newPlayer );
            cout<<"Made player "<<i <<" Computer"<<endl;
        }
    }
    vector<Hand*> hands = deck_->dealCards();

    for(int i = 0; i < hands.size(); ++i) {
        players_.at(i)->setHand(hands.at(i));
    }
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
    return players_.at(playerID);
}

Deck* RoundModel::getDeck() const{
    return deck_;
}

// Add card c to list of played cards in round
void RoundModel::playCard(Card c) {
    //TODO should use copy constructor
    Card* newCard = new Card(c.getSuit(),c.getRank());
    played_cards_.push_back(newCard);
    // notify();
}

vector<Card*> RoundModel::getPlayedCards() const {
    return played_cards_;
}

// Delete player with id playerID and replace with newPlayer
void RoundModel::ragequit(int playerID, Player* newPlayer) {
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

}

void RoundModel::notifyView(){
    notify();
}

void RoundModel::resetCards() {
  topCard_ = -1;
  notify();
}
