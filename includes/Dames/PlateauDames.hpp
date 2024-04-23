#pragma once

#include "../Joueur.hpp"
#include "../Plateau.hpp"

struct PlateauDames final : public Plateau {
  PlateauDames(Joueur &joueur1, Joueur &joueur2);
  virtual ~PlateauDames();

  Joueur *j1, *j2;

  // Renvoie la pièce entre la pièce sélectionné et une position
  Piece *piecePrise(const int destX, const int destY) const;

  // Pareil que deplacementValide mais est utilisable avec des coordonnées
  bool deplacementValideCoor(const int x1, const int y1, const int x2,
                             const int y2) const;

  // Vérifie que le déplacement est valide
  bool deplacementValide(const int destX, const int destY) const;

  // Vérifie que la prise est valide
  bool priseValide(const int destX, const int destY) const;
};
