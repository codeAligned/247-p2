CXX= g++
#CXXFLAGS = -g `/opt/local/bin/pkg-config gtkmm-2.4 --cflags --libs`
CXXFLAGS = -std=c++11 -Wall -O -g `pkg-config gtkmm-2.4 --cflags --libs`
OBJS2 = DeckGUI.o deck_window.o driver.o
EXEC2 = deck

$(EXEC2): $(OBJS2)
	$(CXX) $(OBJS2) $(CXXFLAGS) -o $(EXEC2)

DeckGUI.o: DeckGUI.h DeckGUI.cc
deck_window.o: deck_window.h deck_window.cc
driver.o: deck_window.h driver.cc

clean:
	rm ${OBJS2} ${EXEC2}
