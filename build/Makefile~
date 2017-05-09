CXX=g++
CXXFLAGS=-Wall -D_GLIBCXX_USE_CXX11_ABI=0

SRC=./src
BUILD=./build
INCLUDE=./inc

OBJECTS=$(BUILD)/main.o $(BUILD)/Grille.o $(BUILD)/Ile.o $(BUILD)/IleOuPont.o $(BUILD)/Pont.o

# Construction du programme final
Hashi: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -L./inc/SFML_2.4.2/lib -lsfml-graphics -lsfml-window -lsfml-system -o $@

# Construction de chaque fichier source
$(OBJECTS): $(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -I./inc/SFML_2.4.2/include -o $@

# Nettoyage du dossier source
clean:
	rm -r $(BUILD)/*
