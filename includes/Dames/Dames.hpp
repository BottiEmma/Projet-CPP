#pragma once

#include "../Jeu.hpp"
#include "PlateauDames.hpp"

struct PieceDames;

class Dames final : private Jeu {
  friend std::ostream &operator<<(std::ostream &, const Dames &);

  // Plateau de jeu
  PlateauDames plateau;

  // Joueurs
  Joueur &joueur2;

  // Etape du jeu, pour savoir où on en est
  enum Etape {
    EnJeu = 0,
    Fini,
  };

  // Où l'on se trouve dans le jeu, à quelle étape nous sommes
  enum Etape etape;

  // Permet de transformer une Piece en PieceDames
  const PieceDames *getPiece(const int x, const int y) const;
  const PieceDames *getPiece(Piece *) const;

  // Message à chaque tour du joueur
  const std::string msgTonTour() const;

  // Change de joueur courant
  void changerJoueurCourant();

public:
  Dames(Joueur &joueur1, Joueur &joueur2); // constructor
  virtual ~Dames();                        // destructor

  // Fonction de jeu
  void play() override;

  // Fonction d'évènement
  void event(const int x, const int y) override;

  // Position curseur
  const Position getPosition() const override;

  // Vérifie si une prise est possible pour une pièce donnée
  bool prisePossible(Joueur &joueur) const;
};
