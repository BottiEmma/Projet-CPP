#pragma once

#include "../Jeu.hpp"
#include "PlateauButin.hpp"

struct PieceButin;

class Butin final : private Jeu {
  friend std::ostream &operator<<(std::ostream &, const Butin &);

  // Plateau de jeu
  PlateauButin plateau;

  // Joueurs
  Joueur &joueur2;

  // Etape du jeu, pour savoir où on en est
  enum Etape {
    ChoixJ1 = 0,
    ChoixJ2,
    EnJeu,
    Fini,
  };

  // Où l'on se trouve dans le jeu, à quelle étape nous sommes
  enum Etape etape;

  // Permet de transformer une Piece en PieceButin
  const PieceButin *getPiece(const int x, const int y) const;

  // Message quand les joueurs retirent les pièces au début du jeu
  const std::string msgPieceJaune(const bool erreur = false) const;

  // Message à chaque tour du joueur
  const std::string msgTonTour() const;

  // Message affichant le nombre de points
  const std::string msgPoints(const Position points) const;

  // Position curseur
  const Position getPosition() const override;

  // Change de joueur courant
  void changerJoueurCourant();

  // Renvoie le nombre de points des joueurs 1 et 2
  Position calculpoints(const bool finPartie = false) const;

public:
  Butin(Joueur &joueur1, Joueur &joueur2); // constructor
  virtual ~Butin();                        // destructor

  // Fonction de jeu
  void play() override;

  // Fonction d'évènement
  void event(const int x, const int y) override;
};
