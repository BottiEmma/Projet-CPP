#pragma once

#include "Joueur.hpp"

struct Jeu {
  // Le joueur 1 est toujours celui qui commence
  Jeu(Joueur &j1); // constructor
  virtual ~Jeu();  // destructor

  // Fonction qui fait le jeu
  virtual void play() = 0;

  // Fonction qui fait quelque-chose lors d'un clic gauche
  virtual void event(const int xPos, const int yPos) = 0;

protected:
  // Joueurs, au moins un joueur
  Joueur &joueur1;
  Joueur *joueurCourant;

  // Position du dernier clic du curseur sur l'écran
  PositionMutable posCurseur;

  // Récupère la position du curseur
  // Virtuelle car on aimerais vérifier que le curseur n'est pas en dehors du
  // plateau et on accès à aucun plateau ici
  virtual const Position getPosition() const = 0;
};
