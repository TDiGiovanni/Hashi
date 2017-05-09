#include <fstream>
#include "../inc/Grille.hpp"


int main(int argc, char const *argv[]) {
  // SFML
  sf::RenderWindow window(sf::VideoMode(900, 700), "Hashiwokakero");
  sf::RectangleShape rectangle;
  sf::CircleShape cercle(30.f);
  sf::Text texte;
  sf::Font police;
  police.loadFromFile("../inc/SFML_2.4.2/IntroRustG-Base2Line.otf");
  texte.setFont(police);
  texte.setCharacterSize(50);
  texte.setFillColor(sf::Color::White);
  rectangle.setFillColor(sf::Color::White);
  cercle.setFillColor(sf::Color::Transparent);
  cercle.setOutlineThickness(5);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);
  window.setMouseCursorVisible(true);
  
  Grille grille;
  
  std::ifstream fichier(argv[1], std::ios::in);
  grille.lecture(fichier);
  fichier.close();

  grille.RecupVoisinsPossibles();
  
  /*
  std::cout<<"Avant:\n";
  for (int y = 0; y < grille.getHauteur(); y++) {
    for (int x = 0; x < grille.getLongueur(); x++) {
      if (grille.getUneIleOuUnPont(x,y).getIle() != NULL) {
	std::cout<<"Val: "<<grille.getUneIleOuUnPont(x,y).getIle()->getVal()
		 <<", X: "<<grille.getUneIleOuUnPont(x,y).getIle()->getX()
		 <<", Y: "<<grille.getUneIleOuUnPont(x,y).getIle()->getY()
		 <<", Vp: "<<grille.getUneIleOuUnPont(x,y).getIle()->getVoisinsPossibles().size()<<std::endl;
      }
    }
  }
  std::cout<<"Composantes connexes: "<<grille.getNbreComposantesConnexes()
	   <<", Nb d'iles: "<<grille.getNbIles()
	   <<", Nb d'iles resolues: "<<grille.getNbIlesResolues()<<std::endl<<std::endl;
  */
    
  grille.tracerPonts();
  
  /*
  std::cout<<"Après:\n";
  for (int y = 0; y < grille.getHauteur(); y++) {
    for (int x = 0; x < grille.getLongueur(); x++) {
      if (grille.getUneIleOuUnPont(x,y).getIle() != NULL) {
	std::cout<<"Val: "<<grille.getUneIleOuUnPont(x,y).getIle()->getVal()
		 <<", Vp: "<<grille.getUneIleOuUnPont(x,y).getIle()->getVoisinsPossibles().size()
		 <<", Pp: "<<grille.getUneIleOuUnPont(x,y).getIle()->getPontsPlaces()
		 <<", Resolue? "<<grille.getUneIleOuUnPont(x,y).getIle()->getResolu()<<std::endl;
      }
    }
  }
  std::cout<<"Composantes connexes: "<<grille.getNbreComposantesConnexes()
	   <<", Nb d'iles: "<<grille.getNbIles()
	   <<", Nb d'iles resolues: "<<grille.getNbIlesResolues()<<std::endl<<std::endl;
  */

  while (window.isOpen()) {
    grille.affichage(window,rectangle,cercle,texte);

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
	window.close();
    }

    window.display();
  }
  
  return 0;
}
