#include "Command.h"
#include <cassert>
#include <sstream>
#include <iostream>
using namespace std;

istream &operator>>(istream &in, Command &c){
	c.type = BAD_COMMAND;
	
	string str;
	getline(in, str);
	stringstream ss(str);
	
	string cmd;
//	ss >> cmd;
	cin >> cmd;
	
	if (cmd == "play") {
		c.type = PLAY;
		cin >> c.card;
	} else if (cmd == "discard") {
		c.type = DISCARD;
		cin >> c.card;
	} else if (cmd == "deck") {
		c.type = DECK;
	} else if (cmd == "quit") {
		c.type = QUIT;
	} else if (cmd == "ragequit") {
		c.type = RAGEQUIT;
	}

	assert(!in.fail() && !ss.fail() && c.type != BAD_COMMAND);
	
	return in;
}
