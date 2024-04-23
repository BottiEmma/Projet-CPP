#include "../includes/Piece.hpp"

Piece::Piece(const std::string cat, const int posX, const int posY)
    : selected(false), categorie(cat), x(posX), y(posY) {}

Piece::~Piece() {}

std::ostream &operator<<(std::ostream &out, const Piece &data) {
  out << '"' << data.categorie << '"' << ":" << data.x << "," << data.y;
  return out;
}

void Piece::moveTo(const int destX, const int destY) {
  x = destX;
  y = destY;
}

void Piece::changeSelection() {
  selected = !selected;
}

bool Piece::isSelectionnee() const {
  return selected;
}

const Position Piece::getPos() const {
  return {x, y};
}
