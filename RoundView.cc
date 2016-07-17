/* Thanks Jo Atlee */
#include "observer.h"
#include "RoundView.h"
#include "RoundController.h"
#include "RoundModel.h"
#include "subject.h"
#include "DeckGUI.h"
#include <iostream>
#include <sstream>

using namespace std;

// Creates buttons with labels. Sets butBox elements to have the same size,
// with 10 pixels between widgets
RoundView::RoundView(RoundController *c, RoundModel *m) : model_(m), controller_(c), playedLabel("Played cards:"),
                                      hboxHand( true, 10 ), control_panel( true, 10 ),
                                      new_game( "New Game" ), quit_game( "Quit" ),
                                      nameLabel( "Enter seed:" ), handLabel( "Current Hand:" ) {

    nextCard = 0;
    nextSuit = (Suit) 0;
    nextFace = (Rank) 0;

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

    // Adding player boxes
    player_list = Gtk::manage(new Gtk::HBox());
    vbox.pack_start( *player_list );

    for (int i = 0; i < 4; i++) {

        player_modules[i] = Gtk::manage(new Gtk::VBox());
        player_list->pack_start(*player_modules[i]);

        stringstream ss;
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

    // Adding the hand
    vbox.add( handLabel);
    vbox.add( hboxHand );

    // Initialize 13 empty cards and place them in the box.
    for (int i = 0; i < 13; i++ ) {
        handButtons[i] = new Gtk::Button();
        handButtons[i]->set_image(*(new Gtk::Image(nullCardPixbuf)));
        hboxHand.add(*handButtons[i]);
        handButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &RoundView::onCardClicked), i) );
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

void RoundView::setRagequitButtons(int player_number) {
    for(int i = 0; i < 4; i++) {
        if (i != player_number) {
            ragequitButtons[i]->set_sensitive(false);
        }
        else {
            ragequitButtons[i]->set_sensitive(true);
        }
    }
}

void RoundView::update() {
    int current_player = controller_->getCurrentPlayerID();
    cout<<"UPDATE! showing hand for "<< current_player <<endl;
    setRagequitButtons(current_player);
    showHand(current_player);

    vector<vector<Card*>> playedList = vector<vector<Card*>>();
    playedList.push_back(controller_->getClubs());
    playedList.push_back(controller_->getDiamonds());
    playedList.push_back(controller_->getHearts());
    playedList.push_back(controller_->getSpades());

    for (int i=0;i<4;i++){
        // Initialize 4 empty cards and place them in the box.
        for (int j = 0; j < playedList.at(i).size(); j++ ) {
            int loc = (int)playedList.at(i).at(j)->getRank();
            cout<< 13*i+loc << " is " << *playedList.at(i).at(j)<<endl;
            card[13*i + loc]->set( deck.image( playedList.at(i).at(j)->getRank(), playedList.at(i).at(j)->getSuit() ) );
        }
    }
}

void RoundView::showHand(int player_number) {

    for(int j = 0; j< 4; j++){
        vector<Card*> temphand = controller_->getPlayerHand(j);
        cout<<"Player "<< j << ": "<<endl;
        for (int i = 0; i < temphand.size(); ++i)
        {
            cout<<*temphand.at(i)<<" ";
        }
        cout<<endl;
    }

    vector<Card*> hand = controller_->getPlayerHand(player_number);
    cout<<"Showing "<< player_number<<endl;
     // Change to show player's hand
    for (int i = 0; i < hand.size(); i++ ) {
        Gtk::Image* card_image = new Gtk::Image( deck.image(hand.at(i)->getRank(), hand.at(i)->getSuit()) );
        handButtons[i]->set_image(*(card_image));
        // handButtons[i]->signal_clicked().connect( sigc::bind<int>( sigc::mem_fun(*this, &RoundView::onCardClicked), i) );
    }
    for( int j = hand.size(); j< 13; j++){
        Gtk::Image* card_image = new Gtk::Image( deck.null() );
        handButtons[j]->set_image(*(card_image));
    }
}

void RoundView::onNewGame(){
    cout<<"New Game. Seed = "<< nameField.get_text() <<endl;
    model_->setPlayers(isHuman);
    for(int i = 0; i < 4; i++) {
        toggleCompButtons[i]->hide();
        toggleHumanButtons[i]->hide();
        ragequitButtons[i]->show();
    }

    setRagequitButtons(controller_->who7Spades());
    controller_->setCurrentPlayer(controller_->who7Spades());

    // Make pop up box that says player x's to play
    // int player_number = controller_->who7Spades();
    showHand(controller_->who7Spades());
}

void RoundView::onQuitGame(){
    cout<<"Quit Game."<<endl;
}

void RoundView::onCardClicked(int i){
    cout<<"Card "<<i<< " clicked."<<endl;

    vector<Card*> hand = controller_->getCurrentPlayerHand();
    for(int j = 0; j< 4; j++){
        vector<Card*> temphand = controller_->getPlayerHand(j);
        cout<<"Player "<< j << ": "<<endl;
        for (int i = 0; i < temphand.size(); ++i)
        {
            cout<<*temphand.at(i)<<" ";
        }
        cout<<endl;
    }

    cout<<hand.size()<<endl;

    controller_->determinePlay(controller_->getCurrentPlayer(),*hand.at(i));
}

void RoundView::onRagequit(int i){
    controller_->ragequit(i);
    cout<<"Player "<<i+1<< " ragequit."<<endl;
}

void RoundView::onRoundEnd(){
    cout<<"Round ends."<<endl;
    vector<Player*> players_ = vector<Player*>();
    for (int i = 0; i <= 4; i++) {
      Player* newPlayer = new ComputerPlayer();
      players_.push_back( newPlayer );
    }


    //TEMP CODE SHOULD LET CONTROLLER DO THIS
    displayScore(players_);
}

void RoundView::onHumanToggle(int i){
    cout<<"Player "<<i+1<<" is now Computer."<<endl;
    isHuman[i]=false;
    toggleHumanButtons[i]->hide();
    toggleCompButtons[i]->show();
}

void RoundView::onComputerToggle(int i){
    cout<<"Player "<<i+1<<" is now Human."<<endl;
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
    cout<< "Button clicked"<<endl;
}

void RoundView::displayScore(vector<Player*> players){
    static Gtk::Dialog *dialog = new Gtk::Dialog("End of Round Report");
    Gtk::VBox * reportBox = dialog->get_vbox();

    /* Set title */
    dialog->set_title("Round Report");

    Gtk::Label * scoreReportLabels[4];
    Gtk::Label * discardReportLabels[4];

    // dialog->add(*reportBox);

    for( int i=0;i<4;i++){
        stringstream ss;
        ss<<i+1;
        // cout<<"# players is "<<players.size()<<endl;
        // vector<Card*> discards = players.at(i)->getDiscards();
        // cout<<"# discards is "<<discards.size()<<endl;
        discardReportLabels[i] = new Gtk::Label("Player " +ss.str()+ "'s discards: ");
        // cout<<"Discarding"<<endl;
        // for(int j=0;j<discards.size();j++){
        //     cout<<discards.at(j);
        // }

        scoreReportLabels[i] = new Gtk::Label("Player " +ss.str()+ "'s score: ");
        // cout<<"Score"<<endl;
        // for(int j=0;j<players.size();j++){
        //     cout<<discards.at(j);
        // }
        reportBox->add(*discardReportLabels[i]);
        reportBox->add(*scoreReportLabels[i]);
    }

    dialog->show_all();
}