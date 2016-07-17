/* Created by Jo Atlee on 06/07/09. */


#ifndef ROUND_VIEW_H
#define ROUND_VIEW_H

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/dialog.h>

#include "DeckGUI.h"
#include "observer.h"

class RoundController;
class RoundModel;

class RoundView : public Gtk::Window, public Observer {
public:
    RoundView( RoundController*, RoundModel* );
    void onNewGame();
    void onCardClicked(int);
    void onRagequit(int);
    void onHumanToggle(int);
    void onComputerToggle(int);
    void onQuitGame();
    void onRoundEnd();
    void cleanRound();
    void cleanEndGame();

    void displayScore(std::vector<Player*>);
    void displayWinner();

	virtual ~RoundView();
	virtual void update();	// Observer Pattern: concrete update() method

private:
    void setRagequitButtons(int);
    void showHand(int);
	// Observer Pattern: to access Model accessors without having to downcast subject
	RoundModel *model_;

	// Strategy Pattern member (plus signal handlers)
	RoundController *controller_;

	// Card Images
	DeckGUI deck;    // Knows all of the card pixel buffers.

	int nextCard;
    Suit nextSuit;
    Rank nextFace;

    // Member widgets:
    Gtk::Image * card[52];          // Images to display.
    Gtk::Image * hand[13];          // Images to display.
    // Gtk::Button button;           // Button that will hold an image.
    Gtk::Button * handButtons[13];
    Gtk::HBox * hboxCards[4];
    Gtk::HBox hboxHand;
    Gtk::VBox vbox;
    Gtk::HBox control_panel;
    Gtk::HBox* player_list;
    Gtk::VBox* player_modules[4];
    Gtk::Entry nameField; // Text entry for the user's name
    Gtk::Button new_game;
    Gtk::Button quit_game;
    Gtk::Button * ragequitButtons[4];
    Gtk::Button * toggleHumanButtons[4];
    Gtk::Button * toggleCompButtons[4];

    bool isHuman[4];

    Gtk::Label playedLabel;
    Gtk::Label handLabel;
    Gtk::Label nameLabel;
    Gtk::Label * scoreLabels[4];
    Gtk::Label * discardLabels[4];
    Gtk::Label * pLabels[4];
    Gtk::Frame frame;
	// Signal handlers:
	void nextButtonClicked();
	void resetButtonClicked();

    //temp button:
    Gtk::Button testButton;

}; // View

#endif
