#include "../../includes/Butin/PieceButin.hpp"

PieceButin::PieceButin(const enum Categorie cat, const int posX, const int posY)
    : Piece(to_string(cat), posX, posY), points(cat) {
  this->points = cat;
}

PieceButin::~PieceButin() {}

const std::string PieceButin::to_string(const enum Categorie cat) const {
  switch (cat) {
  case Jaune:
    return "Jaune";
  case Rouge:
    return "Rouge";
  case Noire:
    return "Noire";

  default:
    throw std::logic_error("Catégorie inconnue (PieceButin).");
  }
}

const sf::Color PieceButin::getScreenColor() const {
  switch (points) {
  case Jaune:
    return sf::Color::Yellow;
  case Rouge:
    return sf::Color::Red;
  case Noire:
    return sf::Color::Black;

  default:
    // Ne devrait pas arriver
    return sf::Color::White;
  }
}

int PieceButin::getPoints() const {
  return points;
}
