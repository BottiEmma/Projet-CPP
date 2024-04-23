#pragma once

#include "types.hpp"
#include <SFML/Graphics/Color.hpp>
#include <iostream>

struct Plateau;

class Piece {
  friend std::ostream &operator<<(std::ostream &, const Piece &);
  friend Plateau;

  bool selected;

  // Change l'état de la sélection
  void changeSelection();

protected:
  // Couleur de la pièce ou type d'animal pour Safari par ex
  const std::string categorie;

  // Indices où sont placés les pièces sur le plateau
  int x;
  int y;

public:
  Piece(const std::string categorie, const int x, const int y); // constructor
  virtual ~Piece();                                             // destructor

  // Fonction de déplacement
  void moveTo(const int destX, const int destY);

  // Renvoie la couleur de la pièce pour l'affichage
  virtual const sf::Color getScreenColor() const = 0;

  // Renvoie la position de la pièce
  const Position getPos() const;

  // Renvoie si la pièce est selectionnée
  bool isSelectionnee() const;
};
