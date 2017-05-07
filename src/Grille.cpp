#include <cstdlib>
#include "../inc/Grille.hpp"

// Constructeur
Grille::Grille():
  _hauteur_max(0), _longueur_max(0), _objets_presents(0), _est_resolu(false), _nbre_composantes_connexes(0), _nb_iles(0), _nb_iles_resolues(0)
{}

// Destructeur
Grille::~Grille() {
  for (size_t x = 0; x < _longueur_max; x++) {
    delete(_objets_presents[x]);
  }
  
  delete(_objets_presents);
}


// Accesseurs en lecture
int Grille::getHauteur() const {
    return this->_hauteur_max;
}

int Grille::getLongueur() const {
    return this->_longueur_max;
}

IleOuPont** Grille::getIlesOuPonts() const {
  return this->_objets_presents;
}

IleOuPont Grille::getUneIleOuUnPont(int x, int y) const {
    return this->_objets_presents[x][y];
}

bool Grille::getEstResolu() {
    return this->_est_resolu;
}

int Grille::getNbreComposantesConnexes() {
    return _nbre_composantes_connexes;
}

int Grille::getNbIles() {
  return _nb_iles;
}

int Grille::getNbIlesResolues() {
  return _nb_iles_resolues;
}

// Accesseurs en écriture
void Grille::setHauteur(int h) {
    this->_hauteur_max = h;
}

void Grille::setLongueur(int l) {
  this->_longueur_max = l;
}

void Grille::setUneIleOuUnPont(IleOuPont une_ile_ou_un_pont, int x, int y) {
    this->_objets_presents[x][y]= une_ile_ou_un_pont;
}

void Grille::setEstResolu() {
  if ((_nbre_composantes_connexes == 1) && (_nb_iles == _nb_iles_resolues))
        _est_resolu = true;
    
    else _est_resolu = false;
}

void Grille::resoudreIle(Ile* ile, bool r) {
  if (r) {
    ile->setEstResolu(r);
    _nb_iles_resolues++;
    
    // On applique les règles sur tous les voisins de l'ile récemment résolue, car le nombre de voisins possibles pour ces iles a changé. On en profite pour vider entièrement le vecteur de voisins possibles de l'ile résolue et donc passer son nombre de voisins possibles à 0.
    unsigned int v = 0;
    while (v < ile->getVoisinsPossibles().size()) {
      reglesPonts(ile->getVoisinsPossibles()[v]);
      ile->supprimerUneCaseVoisinsPossibles(v);
    }
  }
}

void Grille::supprimerUnVoisinPossible(Ile* ile, size_t i) {
  ile->supprimerUneCaseVoisinsPossibles(i);
  reglesPonts(ile);
}

void Grille::supprimerUnVoisinPossible(Ile* ile, Ile* voisin) {
  ile->supprimerUneCaseVoisinsPossibles(voisin);
  reglesPonts(ile);
}

void Grille::setNbreComposantesConnexes() {
    if( _nbre_composantes_connexes > 1 )
        _nbre_composantes_connexes = _nbre_composantes_connexes - 1;

    if( _nbre_composantes_connexes == 1 )
        _est_resolu = true;
}

// Fait l'union entre deux composantes connexes
void Grille::unionComposantesConnexes(Ile* chef1, Ile* chef2) {
  if (chef1 != chef2) {
    chef2->setPere(chef1);
    setNbreComposantesConnexes();
  }
}

// Méthodes de récupération fichier et affichage
std::string Grille::enleverEspace(std::string str) {
    std::size_t gauche = str.find_first_not_of("\t");
    std::size_t droite = str.find_last_not_of("\t");

    if (gauche != std::string::npos && droite != std::string::npos)
    {
        return str.substr(gauche, droite - gauche + 1);
    }

    return str;
}

std::string Grille::champDeLecture(std::istream& in) {
    std::string champ;
    int lecture = in.get();
    // Lecture de la ligne jusqu’au prochain ':' ou bien la fin de la ligne
    while (lecture != ':' && lecture != '\n' && in)
    {
        champ += lecture;
        lecture = in.get();
    }

    return enleverEspace(champ);
}

void Grille::ignoreChars(std::istream& in, std::string chars) {
    // Lecture jusqu’au prochain caractère intéressant
    int lecture = in.get();

    while (chars.find(lecture) != std::string::npos && in)
    {
        lecture = in.get();
    }

    // Comme on a lu un caractère de trop, on revient d’un en arrière
    if (in)
    {
        in.unget();
    }
}

void Grille::lecture(std::istream& in) {
    std::string champ;
    int x, y, val;
    Ile* ile;
    bool estCree = false;

    do {
        champ = champDeLecture(in);
        ignoreChars(in,"\t");

        if (champ == "Hauteur") {
            in >> _hauteur_max;
	    in.get();
        }

        if (champ == "Longueur") {
            in >> _longueur_max;
	    in.get();
        }

        if(_hauteur_max && _longueur_max && !estCree) {

            _objets_presents = new IleOuPont*[_longueur_max];
            for (size_t x = 0; x < _longueur_max; x++) {
                _objets_presents[x] = new IleOuPont[_hauteur_max];
            }
            estCree = true;

            // Vidage des iles
            for (size_t y = 0; y < _hauteur_max; y++) {
                for (size_t x = 0; x < _longueur_max; x++) {
                    (this->_objets_presents[x][y]).setIle(NULL);
                    (this->_objets_presents[x][y]).setPont(NULL);
                }
            }
        }

        if (champ == "Grille") {
	  in.seekg(1,std::ios_base::cur);

	  while (champ.size()) {

	      in >> x;
	      in >> y;
	      in >> val;

	        if ( ((unsigned int) x < _longueur_max) && ((unsigned int) y < _hauteur_max) && (y >= 0) && (x >= 0) && (val >= 1) && (val <= 8) ) {
		    ile = new Ile(val,x,y);
		    _objets_presents[x][y].setIle(ile);
		    _nbre_composantes_connexes++;
		    _nb_iles++;
		}
		else {
		  std::cerr<<"Erreur: mauvais choix de coordonnées \n";
		  std::exit(EXIT_FAILURE); // FAIIIIIL
		}
	        in.get();
		champ = champDeLecture(in);
	    }
	}
    } while (champ.size());
}

void Grille::affichage (std::ostream& sortie) const {
  // Limite du haut
  sortie << "+";
  for (size_t i = 1; i <= 2*_longueur_max; i++) {
      sortie << "-";
  }
  sortie<<"+ \n";
  
  for (size_t y = 0; y < _hauteur_max; y++) {
    sortie<<"|";
    for (size_t x = 0; x < _longueur_max; x++) {
      IleOuPont grille = getUneIleOuUnPont(x,y);
      if (grille.getIle() != NULL) {
        sortie<<grille.getIle()->getVal()<<" ";
      }
      else {
	if ( grille.getPont() != NULL ) {
	  if ( grille.getPont()->getNombre() == 1 ) {
	    if ( !(grille.getPont()->getEstVertical()) ) {
	      sortie<<"--";
	    }
	    else {
	      sortie<<"| ";
	    }
	  }
	  else {
	    if ( !(grille.getPont()->getEstVertical()) ) {
	      sortie<<"==";
	    }
	    else {
	      sortie<<"||";
	    }
	  }
	}
	else {
	  sortie<<". ";
	}
      }
    }
    sortie<<"| \n";
  }

  // Limite du bas
  sortie<<"+";
  for (size_t i = 1; i <= 2*_longueur_max; i++) {
    sortie << "-";
  }
  sortie<<"+ \n";
}

// Méthode pour récupérer les voisins possibles de chaque ile
void Grille::RecupVoisinsPossibles() {
  Ile* ile;
  IleOuPont caseActuelle;
  
  for (unsigned int y = 0; y < _hauteur_max; y++) {
    // On réinitialise ile à chaque ligne
    ile = NULL;
    
    for (unsigned int x = 0; x < _longueur_max; x++) {
      caseActuelle =  _objets_presents[x][y];

      // Si une ile est présente
      if (caseActuelle.getIle() != NULL) {
	// Cas de départ
	if ( ile == NULL ) {
	  ile = caseActuelle.getIle();
	}
	// Autres cas
	else {
	  ile->setUnVoisinPossible(caseActuelle.getIle());
	  caseActuelle.getIle()->setUnVoisinPossible(ile);
	  
	  ile = caseActuelle.getIle();
	}
      }
    }
  }

  for (unsigned int x = 0; x < _longueur_max; x++) {
    // On réinitialise ile à chaque colonne
    ile = NULL;
    
    for (unsigned int y = 0; y < _hauteur_max; y++) {
      caseActuelle =  _objets_presents[x][y];

      // Si une ile est présente
      if (caseActuelle.getIle() != NULL) {
	// Cas de départ
	if ( ile == NULL ) {
	  ile = caseActuelle.getIle();
	}
	// Autres cas
	else {
	  ile->setUnVoisinPossible(caseActuelle.getIle());
	  caseActuelle.getIle()->setUnVoisinPossible(ile);
	    
	  ile = caseActuelle.getIle();
	}
      }
    }
  }
}


// Méthode de màj des voisins quand un nouveau pont est créé
void Grille::majVoisinsReels(Pont* pont) {
  // Mise à jour à faire seulement si les iles ne sont pas déjà reliées
  if ( !(pont->getIle1()->dejaVoisin(pont->getIle2())) ) {
    // On commence par ajouter ile1 dans les voisins réels de ile2 et inversément
    pont->getIle1()->setUnVoisinReel(pont->getIle2());
    pont->getIle2()->setUnVoisinReel(pont->getIle1());
    
    // Cas où le pont est vertical
    if ( pont->getEstVertical() ) {
      for (int y = std::min(pont->getIle1()->getY(),pont->getIle2()->getY()) +1;
	   y < std::max(pont->getIle1()->getY(),pont->getIle2()->getY());
	   y++) {
	// On cherche la première ile à droite du pont
	int x1 = pont->getIle1()->getX();
	
	while ( (x1 < getLongueur()-1) && (getUneIleOuUnPont(x1,y).getIle() == NULL) ) {
	  x1++;
	}
	
	// On cherche la première ile à gauche du pont
	int x2 = pont->getIle1()->getX();
	while( (x2 > 0) && (getUneIleOuUnPont(x2,y).getIle() == NULL) ) {
	  x2--;
	}
	
	// Si il existe bien une ile à gauche et à droite, on va supprimer l'un des voisins possibles de l'autre
	if ( (getUneIleOuUnPont(x1,y).getIle() != NULL) && (getUneIleOuUnPont(x2,y).getIle() != NULL) ) {
	  for (size_t v = 0; v < getUneIleOuUnPont(x2,y).getIle()->getVoisinsPossibles().size(); v++) {
	    if ( (getUneIleOuUnPont(x2,y).getIle()->getVoisinsPossibles()[v]->getX() == getUneIleOuUnPont(x1,y).getIle()->getX())
		 && (getUneIleOuUnPont(x2,y).getIle()->getVoisinsPossibles()[v]->getY() == getUneIleOuUnPont(x1,y).getIle()->getY()) ) {
	      // Effacer la case des voisins possibles de x2 si elle correspond bien à l'ile x1
	      getUneIleOuUnPont(x2,y).getIle()->supprimerUneCaseVoisinsPossibles(v);
	    }
	  }
	  
	  for (size_t v = 0 ; v < getUneIleOuUnPont(x1,y).getIle()->getVoisinsPossibles().size(); v++) {
	    if ( (getUneIleOuUnPont(x1,y).getIle()->getVoisinsPossibles()[v]->getX() == getUneIleOuUnPont(x2,y).getIle()->getX())
		 && (getUneIleOuUnPont(x1,y).getIle()->getVoisinsPossibles()[v]->getY() == getUneIleOuUnPont(x2,y).getIle()->getY()) ){
	      // Effacer la case des voisins possibles de x1 si elle correspond bien à l'ile x2
	      getUneIleOuUnPont(x1,y).getIle()->supprimerUneCaseVoisinsPossibles(v);
	    }
	  }
	  reglesPonts(getUneIleOuUnPont(x1,y).getIle());
	  reglesPonts(getUneIleOuUnPont(x2,y).getIle());
	}
      }
    }
    // Cas où le pont est horizontal
    else {
      for (int x = std::min(pont->getIle1()->getX(),pont->getIle2()->getX()) +1;
	   x < std::max(pont->getIle1()->getX(),pont->getIle2()->getX());
	   x++) {
	// On cherche la première ile en dessous du pont
	int y1 = pont->getIle1()->getY();
	while( (y1 < getHauteur()-1) && (getUneIleOuUnPont(x,y1).getIle() == NULL) ) {
	  y1++;
	}
	
	// On cherche la première ile au dessus du pont
	int y2 = pont->getIle1()->getY();
	while( (y2 > 0) && (getUneIleOuUnPont(x,y2).getIle() == NULL) ) {
	  y2--;
	}
	
	// Si il existe bien une ile au dessus et en dessous, on va supprimer l'un des voisins possibles de l'autre
	if( (getUneIleOuUnPont(x,y1).getIle() != NULL) && (getUneIleOuUnPont(x,y2).getIle() != NULL) ) {
	  for (size_t v = 0; v < getUneIleOuUnPont(x,y2).getIle()->getVoisinsPossibles().size(); v++) {
	    if ( getUneIleOuUnPont(x,y2).getIle()->getVoisinsPossibles()[v]->getY() == getUneIleOuUnPont(x,y1).getIle()->getY()
		 && getUneIleOuUnPont(x,y2).getIle()->getVoisinsPossibles()[v]->getX() == getUneIleOuUnPont(x,y1).getIle()->getX() ) {
	      // Effacer la case des voisins possibles de y2 si elle correspond bien à l'ile y1
	      getUneIleOuUnPont(x,y2).getIle()->supprimerUneCaseVoisinsPossibles(v);
	    }
	  }
	  
	  for (size_t v = 0; v < getUneIleOuUnPont(x,y1).getIle()->getVoisinsPossibles().size(); v++) {
	    if ( getUneIleOuUnPont(x,y1).getIle()->getVoisinsPossibles()[v]->getY() == getUneIleOuUnPont(x,y2).getIle()->getY()
		 && getUneIleOuUnPont(x,y1).getIle()->getVoisinsPossibles()[v]->getX() == getUneIleOuUnPont(x,y2).getIle()->getX() ) {
	      // Effacer la case des voisins possibles de y1 si elle correspond bien à l'ile y2
	      getUneIleOuUnPont(x,y1).getIle()->supprimerUneCaseVoisinsPossibles(v);
	    }
	  }
	  reglesPonts(getUneIleOuUnPont(x,y1).getIle());
	  reglesPonts(getUneIleOuUnPont(x,y2).getIle());
	}
      }
    }
  }
}

// Méthode qui traverse toute la grille, et fait appel à reglesPonts dès qu'elle trouve une ile. Recommence tant que la grille n'est pas résolue
void Grille::tracerPonts() {
    for (unsigned int y = 0; y < _hauteur_max; y++) {
      for (unsigned int x = 0; x < _longueur_max; x++) {
	if (getUneIleOuUnPont(x,y).getIle() != NULL) {
	  reglesPonts(getUneIleOuUnPont(x,y).getIle());
	}
      }
    }
}
  
void Grille::reglesPonts(Ile* ile){
  Ile ile2;

  // On commence par vérifier que tous les voisins possibles ne soient pas résolus
  ile->majVoisinsResolus();

  // Règle 1: si Val_restante = 2 * Nb_voisins_possibles, alors doubles ponts partout
  if ( (!(ile->getResolu())) && ((unsigned int) (ile->getVal() - ile->getPontsPlaces()) == 2*(ile->getVoisinsPossibles().size())) ) {
    
    for (size_t i = 0; i < ile->getVoisinsPossibles().size(); i++) {
      ile2 = *(ile->getVoisinsPossibles().at(i));

      // On incrémente le nombre de ponts placés pour les deux iles
      ile->setPontsPlaces(ile->getPontsPlaces()+2);
      getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+2);
      
      // Dans la suite, "getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()" correspond à ile2
      creerPont(ile, getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(), 2);

      // On met à jour les composantes connexes (si besoin)
      unionComposantesConnexes(ile->getChef(),getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getChef());

      // On vérifie si ile2 est "terminée"
      if ( !(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getResolu()) &&
	   getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces() == getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getVal() ) {
	resoudreIle(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(),true);
      }
      // On supprime ile des voisins possibles de ile2, puisqu'on a placé des ponts doubles
      supprimerUnVoisinPossible(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(),ile);
    }
    resoudreIle(ile,true);
  }
  
  // Règle 2: si (Val_restante / 2) + 1 = Nb_voisins_possibles, alors simple pont partout (fonctionne seulement quand Val_restante est impaire)
  if( (!(ile->getResolu())) && ((unsigned int)(ile->getVal() - ile->getPontsPlaces())%2 != 0) && ((((unsigned int)(ile->getVal() - ile->getPontsPlaces()) /2) +1) == ile->getVoisinsPossibles().size()) ) {
    
    for (size_t i = 0; i< ile->getVoisinsPossibles().size(); i++) {
      ile2 = *(ile->getVoisinsPossibles().at(i));

      // S'ils sont déjà reliés, on crée un pont double (et on supprime ile des voisins possibles de ile2 et inversément)
      if ( ile->dejaVoisin(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()) ) {
	       ile->setPontsPlaces(ile->getPontsPlaces()+1);
	getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+1);
	creerPont(ile, getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(), 2);
	supprimerUnVoisinPossible(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(),ile);
	supprimerUnVoisinPossible(ile,getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle());
	i--; // Décrémentation car on réduit la taille du vecteur de voisins possibles
      }
      // Autrement, on crée un pont simple
      else {
       	ile->setPontsPlaces(ile->getPontsPlaces()+1);
	getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+1);
	creerPont(ile, getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(), 1);
	unionComposantesConnexes(ile->getChef(),getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getChef());
      }

      // On vérifie si ile2 est "terminée"
      if ( !(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getResolu()) &&
	   getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces() == getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getVal() ) {
	resoudreIle(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(),true);
      }
    }

    // On vérifie si ile est "terminée"
    if ( ile->getVal() == ile->getPontsPlaces() ) {
      resoudreIle(ile,true);
    }

    // Cas particulier suivant la règle 2 où le nombre de voisins possibles est égal à la valeur restante de l'ile
    if ( (!(ile->getResolu())) && (unsigned int)(ile->getVal() - ile->getPontsPlaces()) == (ile->getVoisinsPossibles().size()) ) {
      for (size_t i = 0; i< ile->getVoisinsPossibles().size() ; i++) {
	ile2 = *(ile->getVoisinsPossibles().at(i));

	// On incrémente le nombre de ponts placés pour les deux iles
	ile->setPontsPlaces(ile->getPontsPlaces()+1);
	getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+1);

	creerPont(ile, getUneIleOuUnPont(ile2.getX(), ile2.getY()).getIle(), 2);

	// On vérifie si ile2 est "terminée"
	if ( !(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getResolu()) &&
	     getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces() == getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getVal() ) {
	  resoudreIle(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(),true);
	}

	// On supprime ile des voisins possibles de ile2 et inversément
	supprimerUnVoisinPossible(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(),ile);
	supprimerUnVoisinPossible(ile,getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle());
	i--; // On décrémente, car on réduit la taille du vecteur de voisins possibles
      }
      resoudreIle(ile,true);
    }
  }
}

void Grille::creerPont(Ile* ile1, Ile* ile2, int nbr_ponts) {
  Pont* pont = new Pont(ile1, ile2, nbr_ponts);
  pont->setEstVertical();

  // Cas du pont horizontal
  if ( pont->getEstVertical() == false ) {
    for (int x = std::min(ile1->getX(),ile2->getX())+1; x < std::max(ile2->getX(),ile1->getX()); x++) {
      // On lie chaque case au pont créé
      _objets_presents[x][ile1->getY()].setPont(pont);
    }
    majVoisinsReels(pont);
  }
  // Cas du pont vertical
  else {
    for (int y = std::min(ile1->getY(),ile2->getY())+1; y < std::max(ile2->getY(),ile1->getY()); y++) {
      // On lie chaque case au pont créé
      _objets_presents[ile1->getX()][y].setPont(pont);
    }
    majVoisinsReels(pont);
  }
}
