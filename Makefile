CXX= g++
CXXFLAGS = -std=c++11 -Wall -O -g `pkg-config gtkmm-2.4 --cflags` -MMD
LDFLAGS=`pkg-config gtkmm-2.4 --libs`
OBJS = Card.o Command.o Hand.o Player.o HumanPlayer.o ComputerPlayer.o Deck.o RoundModel.o \
RoundController.o subject.o DeckGUI.o RoundView.o Game.o main.o
EXEC = straights

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(EXEC)

clean:
	rm $(EXEC) $(OBJS)

# DeckGUI.o: DeckGUI.h DeckGUI.cc
# model.o: subject.h model.h model.cc
# subject.o: subject.h subject.cc
# controller.o: controller.h model.h controller.cc
# view.o: view.h observer.h controller.h model.h subject.h DeckGUI.h view.cc
# main.o: view.h controller.h model.h main.cc
