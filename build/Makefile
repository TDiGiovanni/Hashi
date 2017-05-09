CXX = g++
CXXFLAGS = -Wall -D_GLIBCXX_USE_CXX11_ABI=0

SRC = ../src
INC = ../inc

OBJECTS = Main.o Grille.o Ile.o IleOuPont.o Pont.o

# Construction du programme final
Hashi: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -L../inc/SFML_2.4.2/lib -lsfml-graphics -lsfml-window -lsfml-system -o $@

# Construction de chaque fichier source
$(OBJECTS): %.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -I../inc/SFML_2.4.2/include -o $@

# Nettoyage du dossier source
clean:
	rm -r *
