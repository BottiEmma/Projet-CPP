#pragma once

#include "../Piece.hpp"
#include "Butin.hpp"

struct PieceButin final : public Piece {
  enum Categorie {
    Jaune = 1,
    Rouge = 2,
    Noire = 3,
  };

  PieceButin(const enum Categorie, const int x, const int y);
  virtual ~PieceButin();

  // Couleur sur l'écran
  const sf::Color getScreenColor() const override;

  // Récupère le nombre de points que la pièce vaut
  int getPoints() const;

private:
  int points;

  const std::string to_string(const enum Categorie) const;
};
