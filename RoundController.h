/*Thanks Jo*/
#ifndef ROUND_CONTROLLER_H
#define ROUND_CONTROLLER_H

#include <gtkmm.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "Player.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "RoundModel.h"

class RoundModel;

class RoundController {
public:
    RoundController( RoundModel*, std::vector<Player*>, int seed = 0 );
    // ~RoundController();
    int who7Spades() const;
    void startRound();
    void newRound();
    void newGame();
    void setCurrentPlayer(int);
    int getCurrentPlayerID() const;
    std::vector<Card*> getPlayerHand(int) const;
    std::vector<Card*> calculateLegalPlay(Player*) const;
    std::vector<Player*> getPlayers() const;
    Player* getPlayer(int) const;
    Player* getCurrentPlayer() const;
    std::vector<Card*> getCurrentPlayerHand() const;
   void nextButtonClicked();
   void resetButtonClicked();
    Command playTurn(Player*);
    void executeCommand(Command);
    bool determinePlay(Player*, Card);
    std::vector<Card*> getClubs() const;
    std::vector<Card*> getDiamonds() const;
    std::vector<Card*> getSpades() const;
    std::vector<Card*> getHearts() const;
private:
    void playCard(Player*, Card);
    void discardCard(Player*, Card);
    void ragequit(int);
    bool isLegalPlay(Player*, Card) const;
    int getRoundScore(Player*) const;
    void turnLoop();
    void startTurns();
    void plusPlayerNum(int&);
    void updatePlayerScores();
    std::vector<Card*> filterBySuit(Suit) const;
    int currentPlayer_;
    RoundModel *model_;
}; // Controller


#endif

