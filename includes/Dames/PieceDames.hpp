#pragma once

#include "../Piece.hpp"

struct PieceDames final : public Piece {
  enum Categorie {
    Blanche,
    Noire,
  };

  PieceDames(const enum Categorie, const int x, const int y);
  virtual ~PieceDames();

  // Couleur sur l'écran
  const sf::Color getScreenColor() const override;

  // Catégorie représentée par la pièce
  enum Categorie getCategory() const;

  // Getter pour la piece (dame ou non)
  bool getDame() const;

  // Setter pour la piece (dame ou non)
  void setDame(const bool d);

private:
  const enum Categorie _category;

  // True si la piece est une dame
  bool dame;

  const std::string to_string(const enum Categorie) const;
};
