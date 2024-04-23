#pragma once

#include "../Jeu.hpp"
#include "PieceSafari.hpp"
#include "PlateauSafari.hpp"

class Safari final : private Jeu {
  friend std::ostream &operator<<(std::ostream &, const Safari &);

  // Plateau de jeu
  PlateauSafari plateau;

  // Joueurs
  Joueur &joueur2;
  Joueur *joueur3;

  // Savoir si le joueur courant à déjà déplacer son animal
  // et placer sa barrière
  bool deplacerAnimal, placerBarriere;

  // Vérifie si les joueurs peuvent encore jouer
  int joueursEnVie;

  // Etape du jeu, pour savoir où on en est
  enum Etape {
    ChoixJ1 = 0,
    ChoixJ2,
    ChoixJ3,
    Placement,
    EnJeu,
    Fini,
  };

  // Où l'on se trouve dans le jeu, à quelle étape nous sommes
  enum Etape etape;

  // Barrières posés
  int barrieres;

  // Permet de transformer une Piece en PieceSafari
  PieceSafari *getPiece(const int x, const int y) const;
  const PieceSafari *getPiece(Piece *) const;

  // Position curseur
  const Position getPosition() const override;

  // Change de joueur courant
  void changerJoueurCourant();

  // Message du choix des animaux
  const std::string msgChoixAnimal() const;

  // Message pour le placement des animaux
  const std::string msgPlacement() const;

  // Message à chaque tour de joueur
  const std::string msgTonTour() const;

  // Auxilliaire pour le choix des pièces par le joueur
  void choixJoueur(const Etape prochaineEtape);

public:
  Safari(Joueur &joueur1, Joueur &joueur2,
         Joueur *joueur3 = nullptr); // constructor
  virtual ~Safari();                 // destructor

  // Fonction de jeu
  void play() override;

  // Fonction d'évènement
  void event(const int x, const int y) override;
};
