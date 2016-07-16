/* Thanks Jo Atlee */
#include "observer.h"
#include "RoundView.h"
#include "RoundController.h"
#include "RoundModel.h"
#include "subject.h"
#include "DeckGUI.h"
#include <iostream>
#include <sstream>

// Creates buttons with labels. Sets butBox elements to have the same size,
// with 10 pixels between widgets
RoundView::RoundView(RoundController *c, RoundModel *m) : model_(m), controller_(c), playedLabel("Played cards:"),
                                      hboxHand( true, 10 ), control_panel( true, 10 ),
                                      new_game( "New Game" ), quit_game( "Quit" ),
                                      nameLabel( "Enter seed:" ), handLabel( "Current Hand:" ) {

    nextCard = 0;
    nextSuit = (Suit) 0;
    nextFace = (Rank) 0;

    std::cout<<"Making Glib"<<std::endl;

    const Glib::RefPtr<Gdk::Pixbuf> nullCardPixbuf = deck.null();
    const Glib::RefPtr<Gdk::Pixbuf> cardPixbuf     = deck.image( ACE, SPADE );

    std::cout<<"Got Glib"<<std::endl;

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
    new_game.signal_clicked().connect(sigc::mem_fun( *this, &RoundView::onNewGame));
    control_panel.pack_start( nameLabel );
    control_panel.pack_start( nameField );
    nameField.set_text( "0" );
    control_panel.add(quit_game);
    quit_game.signal_clicked().connect(sigc::mem_fun( *this, &RoundView::onQuitGame));
    testButton.set_label("TEST ROUND END");
    control_panel.pack_start( testButton );
    testButton.signal_clicked().connect(sigc::mem_fun( *this, &RoundView::onRoundEnd));

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
    // button.signal_clicked().connect(sigc::mem_fun( *this, &RoundView::onButtonClicked));

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

        ragequitButtons[i] = new Gtk::Button("Rage");
        ragequitButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &RoundView::onRagequit), i) );
        player_modules[i]->pack_start(*ragequitButtons[i]);


        toggleHumanButtons[i] = new Gtk::Button("Human");
        toggleHumanButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &RoundView::onHumanToggle), i) );
        player_modules[i]->pack_start(*toggleHumanButtons[i]);

        toggleCompButtons[i] = new Gtk::Button("Computer");
        toggleCompButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &RoundView::onComputerToggle), i) );
        player_modules[i]->pack_start(*toggleCompButtons[i]);

        scoreLabels[i] = new Gtk::Label("Score: ");
        player_modules[i]->pack_start(*scoreLabels[i]);
        
        discardLabels[i] = new Gtk::Label("Discards: ");
        player_modules[i]->pack_start(*discardLabels[i]);
    }

    std::cout<<"Tried to make player module"<<std::endl;

    // Adding the hand
    vbox.add( handLabel);
    vbox.add( hboxHand );

    // Initialize 13 empty cards and place them in the box.
    for (int i = 0; i < 13; i++ ) {
        handButtons[i] = new Gtk::Button();
        // Glib::RefPtr<Gdk::Pixbuf> cardImg = deck.image( static_cast<Rank>(i), SPADE );
        // handButtons[i]->set_image(*(new Gtk::Image(cardImg)));
        handButtons[i]->set_image(*(new Gtk::Image(nullCardPixbuf)));
        // card[i] = new Gtk::Image( nullCardPixbuf );
        hboxHand.add(*handButtons[i]);
        // handButtons[i]->signal_clicked().connect(sigc::mem_fun( *this, &RoundView::onButtonClicked));
        handButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &RoundView::onCardClicked), i) );
        // handButtons[i]->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &RoundView::onCardClicked), "button 1") );
    }

    // The final step is to display this newly created widget.
    show_all();

    for(int i=0;i<4;i++){
        isHuman[i]=true;
        toggleCompButtons[i]->hide();
        ragequitButtons[i]->hide();
    }

    // Register view as observer of model
	model_->subscribe(this);

}

RoundView::~RoundView() {}

void RoundView::update() {
  Suit suit = model_->suit();
  Rank face = model_->face();
  // if ( suit == NOSUIT )
  //   card.set( deck.null() );
  // else
  //   card.set( deck.image(face, suit) );
}

void RoundView::onNewGame(){
    std::cout<<"New Game. Seed = "<< nameField.get_text() <<std::endl;  
    for(int i=0;i<4;i++){
        toggleCompButtons[i]->hide();
        toggleHumanButtons[i]->hide();
        ragequitButtons[i]->show();
    }
}

void RoundView::onQuitGame(){
    std::cout<<"Quit Game."<<std::endl;
}

void RoundView::onCardClicked(int i){
    std::cout<<"Card "<<i<< " clicked."<<std::endl;
}

void RoundView::onRagequit(int i){
    std::cout<<"Player "<<i+1<< " ragequit."<<std::endl;
}

void RoundView::onRoundEnd(){
    std::cout<<"Round ends."<<std::endl;
    // Gtk::Dialog roundEndReport = Gtk::Dialog( "End of Round Report", frame, true, true );
    static Gtk::Dialog *dialog = new Gtk::Dialog("End of Round Report");
    Gtk::VBox * reportBox = dialog->get_vbox();
    // gtk_window_set_modal( GTK_WINDOW( dialog ), TRUE );

    /* Set title */
    dialog->set_title("Round Report");
    // gtk_window_set_title( GTK_WINDOW( dialog ), "Round Report" );
    Gtk::Label * scoreReportLabels[4];
    Gtk::Label * discardReportLabels[4];
    
    // dialog->add(*reportBox);

    for( int i=0;i<4;i++){
        std::stringstream ss;
        ss<<i+1;
        discardReportLabels[i] = new Gtk::Label("Player " +ss.str()+ "'s discards: ");
        scoreReportLabels[i] = new Gtk::Label("Player " +ss.str()+ "'s score: ");
        reportBox->add(*discardReportLabels[i]);
        reportBox->add(*scoreReportLabels[i]);

        // dialog->add(*discardReportLabels[i]);
        // dialog->add(*scoreReportLabels[i]);
    }
    dialog->show_all();

    dialog->run();
}

void RoundView::onHumanToggle(int i){
    std::cout<<"Player "<<i+1<<" is now Computer."<<std::endl;
    isHuman[i]=false;
    toggleHumanButtons[i]->hide();
    toggleCompButtons[i]->show();
}

void RoundView::onComputerToggle(int i){
    std::cout<<"Player "<<i+1<<" is now Human."<<std::endl;
    isHuman[i]=true;
    toggleCompButtons[i]->hide();
    toggleHumanButtons[i]->show();
}

void RoundView::onButtonClicked()
{
    card[nextCard]->set(deck.image(nextFace, nextSuit));

    nextCard = (nextCard+1) % 13;
    if( (nextFace+1) == 13)
    {
        nextFace = (Rank) 0;
        nextSuit = (Suit) ((nextSuit + 1) % 4);
    }
    else
    {
        nextFace = (Rank) (nextFace+1);
    }
    std::cout<< "Button clicked"<<std::endl;
}

void RoundView::nextButtonClicked() {
  controller_->nextButtonClicked();
} // RoundView::nextButtonClicked

void RoundView::resetButtonClicked() {
  controller_->resetButtonClicked();
} // RoundView::resetButtonClicked
