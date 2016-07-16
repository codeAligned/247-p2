/* Created by Jo Atlee on 06/07/09. */

#include <gtkmm/main.h>
#include "RoundModel.h"
#include "RoundController.h"
#include "RoundView.h"

int main( int argc, char * argv[] ) {
	Gtk::Main  kit( argc, argv );         // Initialize gtkmm with the command line arguments, as appropriate.
	int seed = 0;

	RoundModel model = RoundModel(seed);                          // Create model
	std::cout<<"Model made"<<std::endl;
    RoundController controller( &model, model.getPlayers());  // Create controller
	std::cout<<"Controller made"<<std::endl;
	RoundView view( &controller, &model );     // Create the view -- is passed handle to controller and model

	std::cout<<"Test"<<std::endl;

	Gtk::Main::run( view );               // Show the window and return when it is closed.
	std::cout<<"View done"<<std::endl;

	return 0;
} // main
