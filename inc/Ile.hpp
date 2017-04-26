#ifndef _ILE_HPP
#define _ILE_HPP

#include <vector>
#include <iostream>


class Ile {
private:
    int _val; // Valeur de l'ile (nombre de ponts qui vont devoir être placés)
    int _abscisse; // Coordonnées en abscisse de l'ile
    int _ordonnee; // Coordonnées en ordonnée de l'ile
    int _ponts_places; // Nombre de ponts déjà placés
    std::vector<Ile*> _voisins_possibles; // Vecteur des iles qui représente tous les voisins possibles
    std::vector<Ile*> _voisins_reels; // Vecteur des iles qui représentent les voisins placés (ne peut pas excéder la valeur de l'ile)
    bool _est_resolu; // vrai si tous les ponts ont été placés, faux sinon
    int _hauteur;
    Ile* _pere;

public:
  // Constructeurs
  Ile();
  Ile(int val, int x, int y);
  Ile(int val, int x, int y, int ponts_places, std::vector<Ile*> voisins_possibles, std::vector<Ile*> voisins_reels, bool est_relie);

  // Destructeur
   ~Ile();
  
  // Accesseurs lecture
  int getVal();
  int getX();
  int getY();
  int getPontsPlaces();
  std::vector<Ile*> getVoisinsPossibles();
  std::vector<Ile*> getVoisinsReels();
  bool getResolu();
  // Accesseurs composantes connexes
  int getHauteur();
  Ile* getPere(); // Récupère le père direct
  Ile* getChef(); // Récupère le plus ancien père

  // Accesseurs écriture
  void setVal(int i);
  void setX(int);
  void setY(int);
  void setPontsPlaces(int);
  void setUnVoisinPossible(Ile*);
  void setEstResolu(bool);
  void setUnVoisinReel(Ile*);
  // Accesseurs composantes connexes
  void setHauteur(int);
  void setPere(Ile*);
  
  // Surcharge
  void affectation(Ile*);
  
  // Méthodes de suppression
  void supprimerUneCaseVoisinsPossibles(size_t);
  void supprimerUneCaseVoisinsPossibles(Ile*);

  // Vérifie si une ile existe déjà dans les voisins réels
  bool dejaVoisin(Ile*);

  // Supprime les iles résolues des voisins possibles
  void majVoisinsResolus();

  // Affichage
  void toString();
  
};

#endif
