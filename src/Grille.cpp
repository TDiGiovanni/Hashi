#include <cstdlib>
#include "../inc/Grille.hpp"

// Constructeur
Grille::Grille(): _n(0), _m(0), _objets_presents(0) {}

// Destructeur
Grille::~Grille() {
  for (size_t i = 0; i < _n; i++) {
    delete[] _objets_presents[i];
  }
  
  delete[] _objets_presents;
}


// Accesseurs en lecture
int Grille::getN() const {
    return this->_n;
}

int Grille::getM() const {
    return this->_m;
}

IleOuPont** Grille::getIlesOuPonts() const {
  return _objets_presents;
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

// Accesseurs en écriture
void Grille::setN(int n) {
    this->_n = n;
}

void Grille::setM(int m) {
    this->_m = m;
}

void Grille::setUneIleOuUnPont(IleOuPont une_ile_ou_un_pont, int x, int y) {
    this->_objets_presents[x][y]= une_ile_ou_un_pont;
}

void Grille::setEstResolu() {
    if ( _nbre_composantes_connexes == 1 )
        _est_resolu = true;
    
    else _est_resolu = false;
}

void Grille::setNbreComposanteConnexes() {
    if( _nbre_composantes_connexes > 1 )
        _nbre_composantes_connexes = _nbre_composantes_connexes - 1;

    if( _nbre_composantes_connexes == 1 )
        _est_resolu = true;
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
            in >> _n;
	    in.get();
        }

        if (champ == "Longueur") {
            in >> _m;
	    in.get();
        }

        if(_n && _m && !estCree) {

            _objets_presents = new IleOuPont*[_n];
            for (size_t i = 0; i < _n; i++) {
                _objets_presents[i] = new IleOuPont[_m];
            }
            estCree = true;

            // Vidage des iles
            for (size_t i = 0; i < _n; i++) {
                for (size_t j = 0; j < _m; j++) {
                    (this->_objets_presents[i][j]).setIle(NULL);
                    (this->_objets_presents[i][j]).setPont(NULL);
                }
            }
        }

        if (champ == "Grille") {
	  in.seekg(1,std::ios_base::cur);

	  while (champ.size()) {

	      in >> x;
	      in >> y;
	      in >> val;

	        if ( ((unsigned int) x < _m) && ((unsigned int) y < _n) && (y >= 0) && (x >= 0) && (val >= 1) && (val <= 8) ) {
		    ile = new Ile(val,x,y);
		    _objets_presents[x][y].setIle(ile);
		    _nbre_composantes_connexes++;
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
    sortie << "+";
  for (size_t i = 1; i <= 2*_m; i++) {
      sortie << "-";
  }
  sortie<< "+ \n";
  for (size_t i = 0; i < _n; i++) {
    sortie << "|";
    for (size_t j = 0; j < _m; j++) {
      IleOuPont grille = getUneIleOuUnPont(j,i);
      if (grille.getIle() != NULL) {
        sortie << grille.getIle()->getVal() << " ";
      }
      else {
	if ( grille.getPont() != NULL ) {
	  if ( grille.getPont()->getNombre() == 1 ) {
	    if ( !(grille.getPont()->getEstVertical()) ) {
	      sortie << "--";
	    }
	    else {
	      sortie << "| ";
	    }
	  }
	  else {
	    if ( !(grille.getPont()->getEstVertical()) ) {
	      sortie << "==";
	    }
	    else {
	      sortie << "||";
	    }
	  }
	}
	else {
	  sortie << ". ";
	}
      }
    }
    sortie <<"| \n";
  }

  sortie << "+";
  for (size_t i = 1; i <= 2*_m; i++) {
    sortie << "-";
  }
  sortie<< "+ \n";
}

// Méthode pour récupérer les voisins possibles de chaque ile
void Grille::RecupVoisinsPossibles() {
  Ile* ile;
  IleOuPont caseActuelle;
  
  for (unsigned int i = 0; i < _n; i++) {
    // On réinitialise ile à chaque ligne
    ile = NULL;
    
    for (unsigned int j = 0; j < _m; j++) {
      caseActuelle =  _objets_presents[i][j];

      // Si une ile est présente
      if (caseActuelle.getIle() != NULL) {
	// Cas de départ
	if ( ile == NULL ) {
	  ile = caseActuelle.getIle();
	}
	// Autres cas
	else {
	  ile->setUnVoisinPossible(caseActuelle.getIle());
	  caseActuelle.getIle()->setUnVoisinPossible(*ile);
	  
	  ile = caseActuelle.getIle();
	}
      }
    }
  }

  for (unsigned int j = 0; j < _m; j++) {
    // On réinitialise ile à chaque colonne
    ile = NULL;
    
    for (unsigned int i = 0; i < _n; i++) {
      caseActuelle =  _objets_presents[i][j];

      // Si une ile est présente
      if (caseActuelle.getIle() != NULL) {
	// Cas de départ
	if ( ile == NULL ) {
	  ile = caseActuelle.getIle();
	}
	// Autres cas
	else {
	  ile->setUnVoisinPossible(caseActuelle.getIle());
	  caseActuelle.getIle()->setUnVoisinPossible(*ile);
	    
	  ile = caseActuelle.getIle();
	}
      }
    }
  }
}


// Méthode de màj des voisins quand un nouveau pont est créé
void Grille::majVoisinsReels(Pont* pont) {
  // On commence par ajouter ile1 dans les voisins réels de ile2 et inversément
  pont->getIle1()->setUnVoisinReel(pont->getIle2());
  pont->getIle2()->setUnVoisinReel(pont->getIle1());

  // Dans le cas où le pont est double, on supprime ile1 des voisins possibles de ile2 et inversément
  if( pont->getNombre() == 2 ) {
    for (size_t i = 0; i < pont->getIle1()->getVoisinsPossibles().size(); i++) {
      if ( (pont->getIle1()->getVoisinsPossibles()[i].getX() == pont->getIle2()->getX())
	   && (pont->getIle1()->getVoisinsPossibles()[i].getY() == pont->getIle2()->getY()) ) {
	// Effacer la case si elle correspond
	pont->getIle1()->supprimerUneCaseVoisinsPossibles(i);
      }
    }
    for (size_t i = 0 ; i < pont->getIle2()->getVoisinsPossibles().size(); i++) {
      if ( (pont->getIle2()->getVoisinsPossibles()[i].getX() == pont->getIle1()->getX())
	   && (pont->getIle2()->getVoisinsPossibles()[i].getY() == pont->getIle1()->getY()) ) {
	// Effacer la case si elle correspond
	pont->getIle2()->supprimerUneCaseVoisinsPossibles(i);
      }
    }
  }

  // Cas où le pont est vertical
  if ( pont->getEstVertical() ) {
    for (int i = std::min(pont->getIle1()->getY(),pont->getIle2()->getY())+1; i < std::max(pont->getIle1()->getY(), pont->getIle2()->getY()); i++) {
      // On cherche la première ile à droite du pont
      int j = i;
      while ( (j < getM()) && (getUneIleOuUnPont(j,i).getIle() == NULL) ) {
	j++;
      }
      
      // On cherche la première ile à gauche du pont
      int k = i;
      while( (k >= 0) && (getUneIleOuUnPont(k,i).getIle() == NULL) ) {
	k--;
      }

      // Si il existe bien une ile à gauche et à droite, on va supprimer l'un des voisins possibles de l'autre
      if ( (getUneIleOuUnPont(j,i).getIle() != NULL) && (getUneIleOuUnPont(k,i).getIle() != NULL) ) {
	for (size_t l = 0; l < getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles().size(); l++) {
	  if ( (getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles()[l].getX() == getUneIleOuUnPont(j,i).getIle()->getX())
	       && (getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles()[l].getY() == getUneIleOuUnPont(j,i).getIle()->getY()) ) {
	    // Effacer la case des voisins possibles de k si elle correspond bien à l'ile j
	    getUneIleOuUnPont(k,i).getIle()->supprimerUneCaseVoisinsPossibles(l);
	  }
	}
        
	for (size_t l=0 ; l< getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles().size(); l++) {
	  if ( (getUneIleOuUnPont(j,i).getIle()->getVoisinsPossibles()[l].getX() == getUneIleOuUnPont(k,i).getIle()->getX())
	       && (getUneIleOuUnPont(j,i).getIle()->getVoisinsPossibles()[l].getY() == getUneIleOuUnPont(k,i).getIle()->getY()) ){
	    // Effacer la case des voisins possibles de j si elle correspond bien à l'ile k
	    getUneIleOuUnPont(j,i).getIle()->supprimerUneCaseVoisinsPossibles(l);
	  }
	}
      }
    }
  }
  // Cas où le pont est horizontal
  else {
    for (int i = std::min(pont->getIle1()->getX(),pont->getIle2()->getX())+1; i < std::max(pont->getIle1()->getX(), pont->getIle2()->getX()); i++) {
      // On cherche la première ile au dessus du pont
      int j = i;
      while( (j >= 0) && (getUneIleOuUnPont(i,j).getIle() == NULL) ) {
	j++;
      }

      // On cherche la première ile en dessous du pont
      int k = 0;
      while( (k < getN()) && (getUneIleOuUnPont(i,k).getIle() == NULL) ) {
	k--;
      }

      // Si il existe bien une ile au dessus et en dessous, on va supprimer l'un des voisins possibles de l'autre
      if( (getUneIleOuUnPont(j,i).getIle() != NULL) && (getUneIleOuUnPont(k,i).getIle() != NULL) ) {
	for (size_t l = 0; l < getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles().size(); l++) {
	  if ( getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles()[l].getY() == getUneIleOuUnPont(j,i).getIle()->getY()
	       && getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles()[l].getX() == getUneIleOuUnPont(j,i).getIle()->getX() ) {
	    // Effacer la case des voisins possibles de k si elle correspond bien à l'ile j
	    getUneIleOuUnPont(k,i).getIle()->supprimerUneCaseVoisinsPossibles(l);
	  }
	}
	
	for (size_t l = 0; l < getUneIleOuUnPont(k,i).getIle()->getVoisinsPossibles().size(); l++) {
	  if ( getUneIleOuUnPont(j,i).getIle()->getVoisinsPossibles()[l].getY() == getUneIleOuUnPont(k,i).getIle()->getY()
	       && getUneIleOuUnPont(j,i).getIle()->getVoisinsPossibles()[l].getX() == getUneIleOuUnPont(k,i).getIle()->getX() ) {
	    // Effacer la case des voisins possibles de j si elle correspond bien à l'ile k
	    getUneIleOuUnPont(j,i).getIle()->supprimerUneCaseVoisinsPossibles(l);
	  }
	}
      }
    }
  }
}

// Méthode qui traverse toute la grille, et fait appel à reglesPonts dès qu'elle trouve une ile.
void Grille::tracerPonts() {
  for (unsigned int i = 0; i < _n; i++) {
    for (unsigned int j = 0; j < _m; j++) {
      if (getUneIleOuUnPont(i,j).getIle() != NULL) {
	reglesPonts(getUneIleOuUnPont(i,j).getIle());
      }
    }
  }
}

void Grille::reglesPonts(Ile* ile){
  Ile ile2;

  // Règle 1: si Val_restante = 2 * Nb_voisins_possibles, alors doubles ponts partout
  if( (!(ile->getRelie())) && ((unsigned int) (ile->getVal() - ile->getPontsPlaces()) == 2*(ile->getVoisinsPossibles().size())) ) {
    
    for(size_t i = 0 ; i < ile->getVoisinsPossibles().size() ; i++) {
      ile2 = ile->getVoisinsPossibles().at(i);
      
      // Dans la suite, "getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()" correspond à ile2
      creerPont(ile, getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(), 2);
      
      ile->setPontsPlaces(ile->getPontsPlaces()+2);
      getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+2);

      // On vérifie si ile2 est terminée
      if ( getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces() == getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getVal() ) {
	getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setEstRelie(true);
      }
    }
    ile->setEstRelie(true);
  }
  
  // Règle 2: si (Val_restante / 2) + 1 = Nb_voisins_possibles, alors simple pont partout (fonctionne seulement quand Val_restante est impaire)
  if( (!(ile->getRelie())) && ((unsigned int)(ile->getVal() - ile->getPontsPlaces())%2 != 0) && ((((unsigned int)(ile->getVal() - ile->getPontsPlaces()) /2) +1) == ile->getVoisinsPossibles().size()) ) {
    
    for (size_t i = 0; i< ile->getVoisinsPossibles().size(); i++) {
      ile2 = ile->getVoisinsPossibles().at(i);
      
      creerPont(ile, getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle(), 1);
      
      ile->setPontsPlaces(ile->getPontsPlaces()+1);
      getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+1);

      // On vérifie si ile2 est "terminée"
      if ( getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces() == getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getVal() ) {
	getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setEstRelie(true);
      }
    }

    // Cas particulier suivant la règle 2
    if ( (unsigned int)(ile->getVal() - ile->getPontsPlaces()) == (ile->getVoisinsPossibles().size()) ) {
      for (size_t i = 0; i< ile->getVoisinsPossibles().size() ; i++) {
	ile2 = ile->getVoisinsPossibles().at(i);
	
	creerPont(ile, getUneIleOuUnPont(ile2.getX(), ile2.getY()).getIle(), 2);
	
	ile->setPontsPlaces(ile->getPontsPlaces()+1);
	getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setPontsPlaces(getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces()+1);

	// On vérifie si ile2 est "terminée"
	if ( getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getPontsPlaces() == getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->getVal() ) {
	  getUneIleOuUnPont(ile2.getX(),ile2.getY()).getIle()->setEstRelie(true);
	}
      }
      ile->setEstRelie(true);
    }
  }
}

void Grille::creerPont(Ile* ile1, Ile* ile2, int nbr_ponts) {
    Pont* pont = new Pont(ile1, ile2, nbr_ponts);
    pont->setEstVertical();
    
    if ( pont->getEstVertical() == false ) {
        for (int i = std::min(ile1->getX(),ile2->getX())+1; i < std::max(ile2->getX(),ile1->getX()); i++) {
            // On lie chaque case au pont créé
            _objets_presents[i][ile1->getY()].setPont(pont);
	}
	majVoisinsReels(pont);
    }
    else {
        for (int i = std::min(ile1->getY(),ile2->getY())+1; i < std::max(ile2->getY(),ile1->getY()); i++) {
            // On lie chaque case au pont créé
            _objets_presents[ile1->getX()][i].setPont(pont);
	}
	majVoisinsReels(pont);
    }
}
