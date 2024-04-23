#pragma once

#include "../includes/Piece.hpp"
#include "types.hpp"
#include <vector>

class Plateau {
  friend std::ostream &operator<<(std::ostream &, const Plateau &);

  // Couleurs des cases
  sf::Color blanc, noir;

protected:
  // Tableau représentant le plateau de jeu
  Piece ***plateau;

  // Taille du plateau
  const int taille;

  // Piece actuellement selectionnée
  Piece *selection;

public:
  Plateau(const int taille,
          const sf::Color couleurCaseBlanche = sf::Color::White,
          const sf::Color couleurCaseNoire = sf::Color::Black); // constructor
  virtual ~Plateau();                                           // destructor

  // Fonction pour afficher le plateau (selon le jeu) vers une sortie
  virtual void afficherPlateau(std::ostream &, const bool debug = false) const;

  // Fonction pour modifier le plateau
  void modifierPlateau(const int x, const int y, Piece *piece) const;

  // Fonction pour bouger une pièce, renvoie les anciennes coordonnées de la
  // pièce bougée (TODO: est-ce utile de renvoyer les anciennes coordonnées ?)
  const Position moveSelection(const int x, const int y);

  // Renvoie une pièce à une position donnnée
  Piece *getPiece(const int x, const int y) const;

  // Prend des coordonnées-écran et renvoie des coordonnées-jeu
  const Position trouveCoordonnees(const int x, const int y) const;

  // Renvoie la taille du plateau
  int getTaille() const;

  // Change la pièce selectionnée
  void modifierSelection(const int x, const int y);

  // Renvoie la liste des pièces du plateau
  std::vector<const Piece *> getPieces() const;
};
