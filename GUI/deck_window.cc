#include "deck_window.h"
#include <iostream>
#include <sstream>

// Sets the horizontal box to have homogeneous spacing (all elements are of the same size), and to put 10 pixels
// between each widget. Initializes the pixel buffer for the null place holder card, and the 10 of spades.
// Puts a frame in the window, and lays out the widgets horizontally. Four widgets are images, the fifth is a button
// with an image in it.
//
// Since widgets cannot be shared, must use pixel buffers to share images.
Deck_Window::Deck_Window() : playedLabel("Played cards:"), hboxHand( true, 10 )
							 , control_panel( true, 10 ), new_game( "New Game" ), quit_game( "Quit" ), nameLabel( "Enter seed:" )
							 , handLabel( "Current Hand:" ){

    nextCard = 0;
    nextSuit = (Suits) 0;
    nextFace = (Faces) 0;
		
	const Glib::RefPtr<Gdk::Pixbuf> nullCardPixbuf = deck.getNullCardImage();
	const Glib::RefPtr<Gdk::Pixbuf> cardPixbuf     = deck.getCardImage( ACE, SPADE );
	
	// Sets the border width of the window.
	set_border_width( 10 );
		
	// Set the look of the frame.
	frame.set_label( "Straights!" );
	frame.set_label_align( Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP );
	frame.set_shadow_type( Gtk::SHADOW_ETCHED_OUT );
	
	// Add the frame to the window. Windows can only hold one widget, same for frames.
	add( frame );
	frame.add(vbox);

	//Add game controls
	vbox.add(control_panel);
	control_panel.add(new_game);
	new_game.signal_clicked().connect(sigc::mem_fun( *this, &Deck_Window::onNewGame));
    control_panel.pack_start( nameLabel );
    control_panel.pack_start( nameField );
    nameField.set_text( "0" );
    control_panel.add(quit_game);
    quit_game.signal_clicked().connect(sigc::mem_fun( *this, &Deck_Window::onQuitGame));

//Adding the played cards section -----------------------------------
    vbox.add(playedLabel);
	// Add the horizontal box for laying out the images to the frame.

    for (int i=0;i<4;i++){
    	hboxCards[i] = Gtk::manage(new Gtk::HBox());
		vbox.add( *hboxCards[i] );
		// Initialize 4 empty cards and place them in the box.
		for (int j = 0+13*i; j < 13*(i+1); j++ ) {
			card[j] = new Gtk::Image( nullCardPixbuf );
			hboxCards[i]->add( *card[j] );
		}
    }
	
	std::cout<<"Finished loop"<<std::endl;
	// Initialize the 5th card and place the image into the button.
	//	card[4] = new Gtk::Image( cardPixbuf );	
	// button.set_image( *card[12] );	
	std::cout<<"Finished setting card"<<std::endl;

	// Attach event listener to button
	// button.signal_clicked().connect(sigc::mem_fun( *this, &Deck_Window::onButtonClicked));
			
	// Add the button to the box.
	// hboxClubs.add( button );
// Adding player boxes
	player_list = Gtk::manage(new Gtk::HBox());
	vbox.pack_start( *player_list );

	for (int i=0;i<4;i++){

		player_modules[i] = Gtk::manage(new Gtk::VBox());
		player_list->pack_start(*player_modules[i]);

		std::stringstream ss;
		ss << i+1;
		pLabels[i] = new Gtk::Label("Player " + ss.str());
	    player_modules[i]->pack_start(*pLabels[i]);

	    scoreLabels[i] = new Gtk::Label("Score: ");
	    player_modules[i]->pack_start(*scoreLabels[i]);

	    ragequitButtons[i] = new Gtk::Button("Rage");
	    ragequitButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &Deck_Window::onRagequit), i) );
	    player_modules[i]->pack_start(*ragequitButtons[i]);
	}

    std::cout<<"Tried to make player module"<<std::endl;

// Adding the hand
	vbox.add( handLabel);
	vbox.add( hboxHand );
	// Initialize 13 empty cards and place them in the box.
	for (int i = 0; i < 13; i++ ) {
		handButtons[i] = new Gtk::Button();
		// Glib::RefPtr<Gdk::Pixbuf> cardImg = deck.getCardImage( static_cast<Faces>(i), SPADE );
		// handButtons[i]->set_image(*(new Gtk::Image(cardImg)));
		handButtons[i]->set_image(*(new Gtk::Image(nullCardPixbuf)));
		// card[i] = new Gtk::Image( nullCardPixbuf );
		hboxHand.add(*handButtons[i]);
		// handButtons[i]->signal_clicked().connect(sigc::mem_fun( *this, &Deck_Window::onButtonClicked));
		handButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &Deck_Window::onCardClicked), i) );
		// handButtons[i]->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &Deck_Window::onCardClicked), "button 1") );
	}
	
	// The final step is to display this newly created widget.
	show_all();
}

Deck_Window::~Deck_Window() {
	//SHOULD DO THIS TODO
	// for (int i = 0; i < 52; i++ ) delete card[i];
	// for (int i = 0; i < 13; i++ ) delete hand[i];
}

void Deck_Window::onNewGame(){
	std::cout<<"New Game. Seed = "<< nameField.get_text() <<std::endl;
}

void Deck_Window::onQuitGame(){
	std::cout<<"Quit Game."<<std::endl;
}

void Deck_Window::onCardClicked(int i){
	std::cout<<"Card "<<i<< " clicked."<<std::endl;
}

void Deck_Window::onRagequit(int i){
	std::cout<<"Player "<<i+1<< " ragequit."<<std::endl;
}

void Deck_Window::onButtonClicked()
{
    card[nextCard]->set(deck.getCardImage(nextFace, nextSuit));

    nextCard = (nextCard+1) % 13;
    if( (nextFace+1) == 13)
    {
        nextFace = (Faces) 0;
        nextSuit = (Suits) ((nextSuit + 1) % 4);
    }
    else
    {
        nextFace = (Faces) (nextFace+1);
    }
    std::cout<< "Button clicked"<<std::endl;
}