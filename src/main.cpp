#include <fstream>
#include "../inc/Grille.hpp"


int main(int argc, char const *argv[]) {
    Grille grille;

    std::ifstream fichier(argv[1], std::ios::in);
    grille.lecture(fichier);
    fichier.close();
    
    grille.RecupVoisinsPossibles();

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
    
    grille.tracerPonts();

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

    grille.affichage(std::cout);

    return 0;
}
