#pragma once

#include "../Piece.hpp"

struct PieceSafari final : public Piece {
  enum Categorie {
    Barriere,
    Elephant,
    Rhinoceros,
    Lion,
  };

  PieceSafari(const enum Categorie, const int x, const int y,
              const int pos2X = -1, const int pos2Y = -1);
  virtual ~PieceSafari();

  // Couleur sur l'écran
  const sf::Color getScreenColor() const override;

  // Catégorie représentée par la pièce
  enum Categorie getCategory() const;

  // Nom de la pièce
  std::string getName() const;

  // Renvoie la position 2 unique aux barrières
  Position getPos2() const;

private:
  const enum Categorie _category;

  // Seconde position, utilisée par les barrières, qui se placent entre 2 pièces
  Position pos2;

  const std::string to_string(const enum Categorie) const;
};
