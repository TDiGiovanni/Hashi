#include <fstream>
#include "../inc/Grille.hpp"


int main(int argc, char const *argv[]) {
    Grille grille;

    std::ifstream fichier(argv[1], std::ios::in);
    grille.lecture(fichier);
    fichier.close();
    
    /* Test de l'affichage des ponts
    Ile* i1 = grille.getUneIleOuUnPont(0,0).getIle();
    Ile* i2 = grille.getUneIleOuUnPont(2,0).getIle();

    grille.creerPont(i1,grille.getUneIleOuUnPont(2,0).getIle(),1);
    */
    
    grille.RecupVoisinsPossibles();

    for (int i = 0; i < grille.getN(); i++) {
      for (int j = 0; j < grille.getM(); j++) {
      if (grille.getUneIleOuUnPont(i,j).getIle() != NULL) {
	std::cout<<grille.getUneIleOuUnPont(i,j).getIle()->getVal()<<" "<<grille.getUneIleOuUnPont(i,j).getIle()->getX()<<" "<<grille.getUneIleOuUnPont(i,j).getIle()->getY()<<" "<<grille.getUneIleOuUnPont(i,j).getIle()->getVoisinsPossibles().size()<<std::endl;
      }
    }
  }
    grille.tracerPonts();

    for (int i = 0; i < grille.getN(); i++) {
      for (int j = 0; j < grille.getM(); j++) {
      if (grille.getUneIleOuUnPont(i,j).getIle() != NULL) {
	std::cout<<grille.getUneIleOuUnPont(i,j).getIle()->getVal()<<" "<<grille.getUneIleOuUnPont(i,j).getIle()->getVoisinsPossibles().size()<<" "<<grille.getUneIleOuUnPont(i,j).getIle()->getPontsPlaces()<<std::endl;
      }
    }
  }

    grille.affichage(std::cout);

    return 0;
}
