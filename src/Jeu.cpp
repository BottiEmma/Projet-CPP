#include "../includes/Jeu.hpp"

Jeu::Jeu(Joueur &j1) : joueur1(j1) {
  joueurCourant = &j1;
}

Jeu::~Jeu() {}
