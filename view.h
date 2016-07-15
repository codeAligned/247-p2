/*
 * MVC example of GTKmm program
 *
 * View class.  Is responsible for buttons (that user clicks) and for displaying
 * the top card of the deck.
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Copyright 2009 UW. All rights reserved.
 *
 */


#ifndef MVC_VIEW_H
#define MVC_VIEW_H

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>

#include "DeckGUI.h"
#include "observer.h"

class Controller;
class Model;

class View : public Gtk::Window, public Observer {
public:
    View( Controller*, Model* );
    void onButtonClicked();
    void onNewGame();
    void onCardClicked(int);
    void onRagequit(int);
    void onQuitGame();
	virtual ~View();
	virtual void update();	// Observer Pattern: concrete update() method

private:
	// Observer Pattern: to access Model accessors without having to downcast subject
	Model *model_;

	// Strategy Pattern member (plus signal handlers)
	Controller *controller_;

	// Card Images
	DeckGUI deck;    // Knows all of the card pixel buffers.

	// Member widgets:
	// Gtk::HBox panels;      // Main window divided into two horizontal panels
	// Gtk::VBox butBox;      // Vertical boxes for stacking buttons vertically
	// Gtk::Button next_button;
	// Gtk::Button reset_button;
	// Gtk::Image card;

    int nextCard;
    Suits nextSuit;
    Faces nextFace;

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
    Gtk::Label playedLabel;
    Gtk::Label handLabel;
    Gtk::Label nameLabel;
    Gtk::Label * scoreLabels[4];
    Gtk::Label * pLabels[4];
    Gtk::Frame frame;
	// Signal handlers:
	void nextButtonClicked();
	void resetButtonClicked();

}; // View

#endif
