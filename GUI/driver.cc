#include "deck_window.h"
#include <gtkmm/main.h>

int main( int argc, char * argv[] ) {
	Gtk::Main  kit( argc, argv );      // Initialize gtkmm with the command line arguments, as appropriate.
	Deck_Window window;                 // Create the window with the image.
	Gtk::Main::run( window );          // Show the window and return when it is closed.
	
	return 0;
} 
