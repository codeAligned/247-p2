#ifndef __DECK_WINDOW_H
#define __DECK_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>

#include "DeckGUI.h"

class Deck_Window : public Gtk::Window {
public:
    Deck_Window();
    void onButtonClicked();
    void onNewGame();
    void onCardClicked(int);
    void onRagequit(int);
    void onQuitGame();
    virtual ~Deck_Window();
	
private:
    DeckGUI deck;             // Knows all of the card pixel buffers.
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
    Gtk::Frame frame;            // Create a nice framed border for the box.
};

#endif
