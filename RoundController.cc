/* thanks jo */
#include "RoundController.h"
using namespace std;

RoundController::RoundController(RoundModel *m, std::vector<Player*>, int seed)  : model_(m) {
    currentPlayer_ = who7Spades();
}

void RoundController::newGame(){
    startRound();
}

void RoundController::setCurrentPlayer(int playerNum){
    currentPlayer_ = playerNum;
}

int RoundController::getCurrentPlayerID() const{
    return currentPlayer_;
}
// Loop through each player's hand and check if they have 7 Spades
int RoundController::who7Spades() const{
    vector<Player*> players = model_->getPlayers();
    for ( int i = 0; i < players.size(); i++ ) {
        vector<Card*> tempHand = players.at(i)->getCards();
        for( int j = 0; j < tempHand.size(); j++) {
            if(tempHand.at(j)->getRank() == SEVEN && tempHand.at(j)->getSuit() == SPADE){
                return i;
            }
        }
    }
    return -1;
}

// RoundController::~RoundController() {}

void RoundController::startRound() {
    for(int i = 0; i < 52; ++i) {
        startTurns();
    }
    updatePlayerScores();
}

void RoundController::startTurns() {
    Player* currentPlayer = getPlayer(currentPlayer_);
    // if (currentPlayer->isHuman()) {
    //     view_->printHumanInfo(getClubs(), getDiamonds(), getHearts(), getSpades(),
    //                           currentPlayer->getCards(), calculateLegalPlay(currentPlayer));
    // }
    // turnLoop();
}

void RoundController::turnLoop() {
    int temp_num = currentPlayer_;
    Player* currentPlayer = getPlayer(currentPlayer_);
    while(temp_num == currentPlayer_) {
        currentPlayer = getPlayer(currentPlayer_);
        Command cmd;
        if(currentPlayer->isHuman()) {
            cmd = currentPlayer->playTurn(this);
        }
        else{
            cmd = playTurn(currentPlayer);
        }
        executeCommand(cmd);
    }
}

void RoundController::executeCommand(Command cmd) {
    Player* currentPlayer = getPlayer(currentPlayer_);
    switch (cmd.type){
        case PLAY:
            if ( isLegalPlay(currentPlayer, cmd.card) ) {
                cout<<"Played: "<<cmd.card<<endl;
                playCard(currentPlayer, cmd.card);
                // view_->printPlayMessage(player_number, cmd.card);
                plusPlayerNum(currentPlayer_);
                model_->notifyView();
            }
            else {
                // view_->printIllegalPlay();
                cout<<"Illegal Play"<<endl;
                // executeCommand(currentPlayer->playTurn(this));
            }
            break;
        case DISCARD:
            if (calculateLegalPlay(currentPlayer).size() == 0) {
                cout<<"Discarded: "<<cmd.card<<endl;
                discardCard(currentPlayer, cmd.card);
                // view_->printDiscardMessage(player_number, cmd.card);
                plusPlayerNum(currentPlayer_);
                model_->notifyView();
            }
            else {
                // view_->printBadDiscard();
                cout<<"Illegal Discard"<<endl;
                // executeCommand(currentPlayer->playTurn(this));
            }
            break;
        case DECK:
            // view_->printDeck(model_->getDeck());
            break;
        case QUIT:
            exit(0);
        case RAGEQUIT:
            // view_->printRageQuitMessage(player_number);
            ragequit(currentPlayer_);
            break;
        default:
            throw "Bad Command";
    }
}

int RoundController::getRoundScore(Player* p) const {
    int roundScore = 0;
    vector<Card*> discards = p->getDiscards();
    for (int i = 0; i < discards.size(); ++i) {
        roundScore += discards.at(i)->getRank()+1;
    }
    return roundScore;
}

void RoundController::updatePlayerScores() {
    vector<Player*> players = model_->getPlayers();
    for (int i = 0; i < players.size(); ++i) {
        Player* current_player = players.at(i);
        int current_score = current_player->getScore();
        int round_score = getRoundScore( current_player );
        // view_->printPlayerScore(current_player->getDiscards(), current_score, round_score, i+1);
        players.at(i)->setScore(current_score + round_score);
    }
}

Player* RoundController::getPlayer(int playerID) const{
    return model_->getPlayer(playerID);
}

Player* RoundController::getCurrentPlayer() const{
    return model_->getPlayer(currentPlayer_);
}

void RoundController::playCard(Player* p, Card c){
    model_->playCard(c);
    p->playCard(c);
}

void RoundController::discardCard(Player* p, Card c){
    p->discardCard(c);
}

std::vector<Card*> RoundController::getPlayerHand(int player_number) const {
    Player* p = model_->getPlayer(player_number);
    return p->getCards();
}
std::vector<Card*> RoundController::getCurrentPlayerHand() const {
    Player* p = model_->getPlayer(currentPlayer_);
    return p->getCards();
}

//true = executed
//false = play was illegal
bool RoundController::determinePlay(Player* p, Card c){
    Command cmd = Command();

    if( calculateLegalPlay(p).size()==0 ){
        cmd.type = DISCARD;
        cmd.card = c;
        executeCommand(cmd);
        return true;
    }
    else if(isLegalPlay(p,c)){
        cmd.type = PLAY;
        cmd.card = c;
        executeCommand(cmd);
        return true;
    }
    cout << "This is not a legal play" << endl;
    return false;
}

void RoundController::playComputerTurn(int player_number) {
    Player* current_player = getPlayer(player_number);
    executeCommand(playTurn(current_player));
}

bool RoundController::playerIsHuman(int player_number) {
    Player* current_player = getPlayer(player_number);
    return current_player->isHuman();
}

// Returns vector of legal plays for a player
vector<Card*> RoundController::calculateLegalPlay(Player* p) const{
    vector<Card*> played = model_->getPlayedCards();
    vector<Card*> hand = p->getCards();
    vector<Card*> ret = vector<Card*>();

    // If first turn, only legal play is 7 spades
    if(model_->getPlayedCards().size() == 0){
        for( int i = 0; i < hand.size(); ++i ){
            if(hand.at(i)->getSuit()== SPADE && hand.at(i)->getRank() == SEVEN){
                ret.push_back(hand.at(i));
                return ret;
            }
        }
    }

    // Check player's hand and round's played cards for legal plays
    for(int i = 0; i < hand.size(); ++i ) {

        for(int j = 0; j < played.size(); ++j) {
            if(played.at(j)->getSuit()==hand.at(i)->getSuit() && abs(played.at(j)->getRank() - hand.at(i)->getRank())<=1 ){
                ret.push_back(hand.at(i));
                break;
            }
            else if(hand.at(i)->getRank()==SEVEN){
                ret.push_back(hand.at(i));
                break;
            }
        }

    }
    return ret;
}

bool RoundController::isLegalPlay(Player* p, Card c) const{
    vector<Card*> legal_plays = calculateLegalPlay(p);
    for (int i = 0; i < legal_plays.size(); ++i) {
        if (c == *(legal_plays.at(i)) )
            return true;
    }
    return false;
}

vector<Card*> RoundController::getClubs() const {
    return filterBySuit(CLUB);
}
vector<Card*> RoundController::getDiamonds() const {
    return filterBySuit(DIAMOND);
}
vector<Card*> RoundController::getHearts() const {
    return filterBySuit(HEART);
}
vector<Card*> RoundController::getSpades() const {
    return filterBySuit(SPADE);
}
bool compareCards(Card* a, Card* b) { return (a->getRank() < b->getRank()); };

vector<Card*> RoundController::filterBySuit(Suit suit) const {
    vector<Card*> temp = model_->getPlayedCards();
    vector<Card*> ret = vector<Card*>();
    for(int i = 0; i < temp.size(); ++i ){
        if(temp.at(i)->getSuit() == suit){
            ret.push_back(temp.at(i));
        }
    }
    sort(ret.begin(),ret.end(),compareCards);
    return ret;
}

Command RoundController::playTurn(Player* player) {
    ComputerPlayer* playerTemp = static_cast<ComputerPlayer*>(player);
    return playerTemp->playTurn(this);
}

void RoundController::ragequit(int player_number) {
    Player* p = model_->getPlayer(player_number);
    ComputerPlayer* newPlayer = new ComputerPlayer(*p);
    model_->ragequit(player_number, newPlayer);
}

void RoundController::newRound() {
    model_->newRound();
    // view_->printNewRound(who7Spades());
}

vector<Player*> RoundController::getPlayers() const{
    return model_->getPlayers();
}

void RoundController::plusPlayerNum(int &player_number) {
    cout<<"Before: "<<player_number<<endl;
    player_number = player_number+1;
    if(player_number == 4){
        player_number = 0;
    }
    cout<<"After: "<<player_number<<endl;
}

void RoundController::nextButtonClicked() {
     model_->nextCard();
}


void RoundController::resetButtonClicked() {
     model_->resetCards();
}
