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

    std::cout<<"Avant:\n";
    for (int i = 0; i < grille.getHauteur(); i++) {
      for (int j = 0; j < grille.getLongueur(); j++) {
      if (grille.getUneIleOuUnPont(i,j).getIle() != NULL) {
	std::cout<<"Val:"<<grille.getUneIleOuUnPont(i,j).getIle()->getVal()<<" X:"<<grille.getUneIleOuUnPont(i,j).getIle()->getX()<<" Y:"<<grille.getUneIleOuUnPont(i,j).getIle()->getY()<<" Vp:"<<grille.getUneIleOuUnPont(i,j).getIle()->getVoisinsPossibles().size()<<std::endl;
      }
    }
  }
    
    grille.tracerPonts();

    std::cout<<"Après:\n";
    for (int i = 0; i < grille.getHauteur(); i++) {
      for (int j = 0; j < grille.getLongueur(); j++) {
      if (grille.getUneIleOuUnPont(i,j).getIle() != NULL) {
	std::cout<<"Val:"<<grille.getUneIleOuUnPont(i,j).getIle()->getVal()<<" Pp:"<<grille.getUneIleOuUnPont(i,j).getIle()->getPontsPlaces()<<std::endl;
      }
    }
  }

    grille.affichage(std::cout);

    std::cout<<grille.getUneIleOuUnPont(0,3).getIle()->getVal();

    return 0;
}
