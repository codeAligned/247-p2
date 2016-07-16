/* Created by Jo Atlee on 06/07/09. */

#include <gtkmm/main.h>
#include "RoundModel.h"
#include "RoundController.h"
#include "RoundView.h"

vector<Player*> createPlayers(){
	vector<Player*> players_ = vector<Player*>();
	for (int i = 1; i <= 4; ++i) {
        char playerType;
        std::cout << "Is player " << i << " a human(h) or a computer(c)?" << std::endl << ">";
        std::cin >> playerType;
        if (playerType == 'c') {
            players_.push_back( new ComputerPlayer() );
        }
        else if (playerType == 'h'){
            players_.push_back( new HumanPlayer() );
        }
        else {
            throw "Response not c or h";
        }
    }
    return players_;
}

int main( int argc, char * argv[] ) {
	Gtk::Main  kit( argc, argv );         // Initialize gtkmm with the command line arguments, as appropriate.
	int seed = 0;
	
	std::vector<Player*> players= createPlayers();
    Deck* deck = new Deck(seed);

	RoundModel model = RoundModel(players, deck);                          // Create model
	std::cout<<"Model made"<<std::endl;
    RoundController controller( &model, model.getPlayers());  // Create controller
	std::cout<<"Controller made"<<std::endl;
	RoundView view( &controller, &model );     // Create the view -- is passed handle to controller and model

	std::cout<<"Test"<<std::endl;

	Gtk::Main::run( view );               // Show the window and return when it is closed.
	std::cout<<"View done"<<std::endl;

	return 0;
} // main
