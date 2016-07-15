#ifndef _COMPUTERPLAYER_
#define _COMPUTERPLAYER_

class RoundController;
class Player;
#include <vector>
#include "RoundController.h"

class ComputerPlayer: public Player {
public:
    ComputerPlayer();
    ComputerPlayer(Player&);
    Command playTurn(RoundController*);
    bool isHuman() const;
private:
};

#endif
