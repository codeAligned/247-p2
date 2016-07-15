/* Thanks Jo Atlee */
#include "observer.h"
#include "view.h"
#include "controller.h"
#include "model.h"
#include "subject.h"
#include "DeckGUI.h"
#include <iostream>
#include <sstream>

// Creates buttons with labels. Sets butBox elements to have the same size,
// with 10 pixels between widgets
View::View(Controller *c, Model *m) : model_(m), controller_(c), playedLabel("Played cards:"),
                                      hboxHand( true, 10 ), control_panel( true, 10 ),
                                      new_game( "New Game" ), quit_game( "Quit" ),
                                      nameLabel( "Enter seed:" ), handLabel( "Current Hand:" ) {

    nextCard = 0;
    nextSuit = (Suits) 0;
    nextFace = (Faces) 0;

    const Glib::RefPtr<Gdk::Pixbuf> nullCardPixbuf = deck.null();
    const Glib::RefPtr<Gdk::Pixbuf> cardPixbuf     = deck.image( ACE, SPADE );

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
    new_game.signal_clicked().connect(sigc::mem_fun( *this, &View::onNewGame));
    control_panel.pack_start( nameLabel );
    control_panel.pack_start( nameField );
    nameField.set_text( "0" );
    control_panel.add(quit_game);
    quit_game.signal_clicked().connect(sigc::mem_fun( *this, &View::onQuitGame));

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
    //  card[4] = new Gtk::Image( cardPixbuf );
    // button.set_image( *card[12] );
    std::cout<<"Finished setting card"<<std::endl;

    // Attach event listener to button
    // button.signal_clicked().connect(sigc::mem_fun( *this, &View::onButtonClicked));

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

        ragequitButtons[i] = new Gtk::Button("Human");
        ragequitButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &View::onRagequit), i) );
        player_modules[i]->pack_start(*ragequitButtons[i]);
    }

    std::cout<<"Tried to make player module"<<std::endl;

    // Adding the hand
    vbox.add( handLabel);
    vbox.add( hboxHand );

    // Initialize 13 empty cards and place them in the box.
    for (int i = 0; i < 13; i++ ) {
        handButtons[i] = new Gtk::Button();
        // Glib::RefPtr<Gdk::Pixbuf> cardImg = deck.image( static_cast<Faces>(i), SPADE );
        // handButtons[i]->set_image(*(new Gtk::Image(cardImg)));
        handButtons[i]->set_image(*(new Gtk::Image(nullCardPixbuf)));
        // card[i] = new Gtk::Image( nullCardPixbuf );
        hboxHand.add(*handButtons[i]);
        // handButtons[i]->signal_clicked().connect(sigc::mem_fun( *this, &View::onButtonClicked));
        handButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &View::onCardClicked), i) );
        // handButtons[i]->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &View::onCardClicked), "button 1") );
    }

    // The final step is to display this newly created widget.
    show_all();

    // Register view as observer of model
	model_->subscribe(this);

}

View::~View() {}

void View::update() {
  Suits suit = model_->suit();
  Faces face = model_->face();
  // if ( suit == NOSUIT )
  //   card.set( deck.null() );
  // else
  //   card.set( deck.image(face, suit) );
}

void View::onNewGame(){
    std::cout<<"New Game. Seed = "<< nameField.get_text() <<std::endl;
}

void View::onQuitGame(){
    std::cout<<"Quit Game."<<std::endl;
}

void View::onCardClicked(int i){
    std::cout<<"Card "<<i<< " clicked."<<std::endl;
}

void View::onRagequit(int i){
    std::cout<<"Player "<<i+1<< " ragequit."<<std::endl;
}

void View::onButtonClicked()
{
    card[nextCard]->set(deck.image(nextFace, nextSuit));

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

void View::nextButtonClicked() {
  controller_->nextButtonClicked();
} // View::nextButtonClicked

void View::resetButtonClicked() {
  controller_->resetButtonClicked();
} // View::resetButtonClicked
