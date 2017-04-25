#ifndef _GRILLE_H
#define _GRILLE_H

#include <string>
#include "IleOuPont.hpp"
#include "Pont.hpp"

class Grille{
private:
    unsigned int _hauteur_max; // Hauteur donnée dans le fichier texte
    unsigned int _longueur_max; // Longueur donnée dans le fichier texte
    IleOuPont** _objets_presents; // Vecteur à 2 dimensions des iles ou des ponts dans la grille
  bool _est_resolu; // Vrai si toutes les iles sont résolues
  int _nbre_composantes_connexes;
  int _nb_iles;
  int _nb_iles_resolues;

public:
    // Constructeur
    Grille(); // Par défaut

    // Destructeur
    ~Grille();

  // Accesseurs lecture
  int getHauteur() const;
  int getLongueur() const;
  IleOuPont** getIlesOuPonts() const;
  IleOuPont getUneIleOuUnPont(int, int) const;
  bool getEstResolu();
  // Composantes connexes
  int getNbreComposantesConnexes();
  int getNbIles();
  int getNbIlesResolues();
  
  // Accesseurs écriture
  void setHauteur(int);
  void setLongueur(int);
  void setUneIleOuUnPont(IleOuPont, int, int);
  void setEstResolu();
  // Composantes connexes
  void setNbreComposantesConnexes();
  void unionComposantesConnexes(Ile*, Ile*);

  // Construction de la grille
  std::string enleverEspace(std::string);
  std::string champDeLecture(std::istream&);
  void ignoreChars(std::istream&, std::string);
  void lecture(std::istream&); // Reconstruit la grille à partir du fichier
  // Affichage
  void affichage(std::ostream&) const; // Affichage sur un flux de sortie

  // Voisins d'Iles
  void RecupVoisinsPossibles(); // Récupère les voisins possibles de chaque ile
  void majVoisinsReels(Pont*); // Méthode de màj quand on crée un pont
  void tracerPonts();
  void reglesPonts(Ile*); // Enumération des cas possibles
  void creerPont(Ile*, Ile*, int);

  void resoudreIle(Ile*, bool);
  void supprimerUnVoisinPossible(Ile*, size_t);
  void supprimerUnVoisinPossible(Ile*, Ile*);
};

#endif
