#include "../../includes/Dames/PieceDames.hpp"

PieceDames::PieceDames(const enum Categorie cat, const int posX, const int posY)
    : Piece(to_string(cat), posX, posY), _category(cat), dame(false) {
  std::cout << "pièce - " << categorie << "\n";
}

PieceDames::~PieceDames() {}

enum PieceDames::Categorie PieceDames::getCategory() const {
  return _category;
}

bool PieceDames::getDame() const {
  return dame;
}

void PieceDames::setDame(const bool d) {
  dame = d;
}

const std::string PieceDames::to_string(const enum Categorie cat) const {
  switch (cat) {
  case Blanche:
    return "Blanche";
  case Noire:
    return "Noire";

  default:
    throw std::logic_error("Catégorie inconnue (PieceDames).");
  }
}

const sf::Color PieceDames::getScreenColor() const {
  switch (_category) {
  case Blanche:
    return sf::Color::White;
  case Noire:
    return sf::Color::Black;

  default:
    // Ne devrait pas arriver
    return sf::Color::White;
  }
}
