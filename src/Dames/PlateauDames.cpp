#include "../../includes/Dames/PlateauDames.hpp"
#include "../../includes/Dames/PieceDames.hpp"

PlateauDames::PlateauDames(Joueur &joueur1, Joueur &joueur2)
    : Plateau(10, sf::Color(255, 205, 160), sf::Color(210, 140, 70)),
      j1(&joueur1), j2(&joueur2) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 4; j++) {
      if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) {
        plateau[i][j] = new PieceDames(PieceDames::Noire, i, j);
        j1->ajoutPiece(plateau[i][j]);
      }
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 6; j < 10; j++) {
      if ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) {
        plateau[i][j] = new PieceDames(PieceDames::Blanche, i, j);
        j2->ajoutPiece(plateau[i][j]);
      }
    }
  }
}

PlateauDames::~PlateauDames() {}

Piece *PlateauDames::piecePrise(const int x, const int y) const {
  const Position posInitiale = selection->getPos();
  if (x == posInitiale.first + 2 && y == posInitiale.second + 2) {
    return plateau[posInitiale.first + 1][posInitiale.second + 1];
  }
  if (x == posInitiale.first - 2 && y == posInitiale.second - 2) {
    return plateau[posInitiale.first - 1][posInitiale.second - 1];
  }
  if (x == posInitiale.first + 2 && y == posInitiale.second - 2) {
    return plateau[posInitiale.first + 1][posInitiale.second - 1];
  }
  if (x == posInitiale.first - 2 && y == posInitiale.second + 2) {
    return plateau[posInitiale.first - 1][posInitiale.second + 1];
  }
  return nullptr;
}

bool PlateauDames::deplacementValideCoor(const int x1, const int y1,
                                         const int x2, const int y2) const {
  if ((x2 == x1 + 1 && y2 == y1 + 1) || (x2 == x1 - 1 && y2 == y1 - 1) ||
      (x2 == x1 + 1 && y2 == y1 - 1) || (x2 == x1 - 1 && y2 == y1 + 1)) {
    return true;
  }
  return false;
}

bool PlateauDames::deplacementValide(const int x, const int y) const {
  if (!selection) {
    return false;
  }

  Position posInitiale = selection->getPos();
  return deplacementValideCoor(posInitiale.first, posInitiale.second, x, y);
}

bool PlateauDames::priseValide(const int x, const int y) const {
  if (!selection) {
    return false;
  }

  Position posInitiale = selection->getPos();
  if ((x == posInitiale.first + 2 && y == posInitiale.second + 2) ||
      (x == posInitiale.first - 2 && y == posInitiale.second - 2) ||
      (x == posInitiale.first + 2 && y == posInitiale.second - 2) ||
      (x == posInitiale.first - 2 && y == posInitiale.second + 2)) {

    return true;
  }
  return false;
}
