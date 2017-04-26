#include "../inc/Ile.hpp"

// Constructeurs
Ile::Ile():
  _val(0), _abscisse(0), _ordonnee(0), _ponts_places(0), _est_resolu(false) // Constructeur par défaut
{}

Ile::Ile(int val, int x, int y):
  _val(val), _abscisse(x), _ordonnee(y), _ponts_places(0), _est_resolu(false)
{}

Ile::Ile(int val, int x, int y, int p_p, std::vector<Ile*> v_p, std::vector<Ile*> v_r, bool est_r):
_val(val), _abscisse(x), _ordonnee(y), _ponts_places(p_p), _voisins_possibles(v_p), _voisins_reels(v_r), _est_resolu(est_r)
{}

// Destructeur
Ile::~Ile() {
  _voisins_reels.clear();
  
  _voisins_possibles.clear();
}

// Accesseurs en lecture
int Ile::getVal() {
    return _val;
}

int Ile::getX() {
    return _abscisse;
}

int Ile::getY() {
    return _ordonnee;
}

int Ile::getPontsPlaces() {
    return _ponts_places;
}

std::vector<Ile*> Ile::getVoisinsPossibles() {
    return _voisins_possibles;
}

std::vector<Ile*> Ile::getVoisinsReels() {
    return _voisins_reels;
}

bool Ile::getResolu() {
    return _est_resolu;
}

int Ile::getHauteur() {
    return _hauteur;
}

Ile* Ile::getPere() {
    return _pere;
}
Ile* Ile::getChef() {
    if ( this->_pere == NULL ) {
        return this;
    }
    else {
      this->_pere = this->_pere->getChef();
      return _pere;
    }
}

// Accesseurs en écriture
void Ile::setVal(int i) {
    _val = _val - i;
}

void Ile::setX(int x) {
    _abscisse = x;
}

void Ile::setY(int y) {
    _ordonnee = y;
}

void Ile::setPontsPlaces(int n) {
    _ponts_places = n;
}

void Ile::setUnVoisinPossible(Ile* ile) {
    _voisins_possibles.push_back(ile);
}

void Ile::setEstResolu(bool r) {
  if (r == true)
    _est_resolu = r;
}

void Ile::setUnVoisinReel(Ile* ile) {
    _voisins_reels.push_back(ile);
}

void Ile::setHauteur(int h) {
    _hauteur = h;
}

void Ile::setPere(Ile* ile) {
    _pere = ile;
}


// Surcharge affectation par méthode
void Ile::affectation(Ile* ile) {
    this->_val= ile->getVal();
    this->_abscisse= ile->getX();
    this->_ordonnee= ile->getY();
    this->_ponts_places= ile->getPontsPlaces();
    this->_voisins_possibles= ile->getVoisinsPossibles();
    this->_voisins_reels= ile->getVoisinsReels();
    this->_est_resolu= ile->getResolu();
}

// Méthodes de suppression
void Ile::supprimerUneCaseVoisinsPossibles(size_t i){
    if ( i >= 0 && i < _voisins_possibles.size() ) {
        _voisins_possibles.erase(_voisins_possibles.begin()+i);
    }
}

void Ile::supprimerUneCaseVoisinsPossibles(Ile* ile){
  for (size_t i = 0; i < _voisins_possibles.size(); i++) {
    if (_voisins_possibles[i]->getX() == ile->getX()
     && _voisins_possibles[i]->getY() == ile->getY()) {
      _voisins_possibles.erase(_voisins_possibles.begin()+i);
    }
  }
}

// Vérifie si l'ile existe déjà dans les voisins réels
bool Ile::dejaVoisin(Ile* ile) {
  for (unsigned int r = 0; r < _voisins_reels.size(); r++) {
    if (ile == _voisins_reels.at(r))
      return true;
  }

  return false;
}

// Supprime les iles résolues des voisins possibles
void Ile::majVoisinsResolus() {
  for (unsigned int i = 0; i < _voisins_possibles.size(); i++)
    if ( _voisins_possibles[i]->getResolu() )
      supprimerUneCaseVoisinsPossibles(i);
}

// Méthode d'affichage
void Ile::toString() {
  std::cout<<"Valeur: "<<_val<<", X: "<<_abscisse<<", Y: "<<_ordonnee<<std::endl;
}
