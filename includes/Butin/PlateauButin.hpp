#pragma once

#include "../Plateau.hpp"
#include <vector>

struct PlateauButin final : public Plateau {
  PlateauButin();
  virtual ~PlateauButin();

  // Renvoie la liste des pièces entre la pièce sélectionné et une position
  const std::vector<const Piece *> cheminPieces(const int destX,
                                                const int destY) const;

  // Vrai si la pièce sélectionnée est jaune
  bool selectionJaune() const;

  // Vrai si un coup est possible
  bool coupsPossible() const;

private:
  // Renvoie la liste des pièces entre deux position
  const std::vector<const Piece *> cheminPieces(const int depX, const int depY,
                                                const int destX,
                                                const int destY) const;
};
