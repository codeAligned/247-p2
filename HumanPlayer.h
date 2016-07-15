#ifndef _HUMANPLAYER_
#define _HUMANPLAYER_

#include <vector>
#include "Player.h"

class HumanPlayer: public Player {
public:
    HumanPlayer();
    Command playTurn(RoundController*);
    bool isHuman() const;
private:

};

#endif
