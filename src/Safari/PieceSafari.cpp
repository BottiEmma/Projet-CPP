#include "../../includes/Safari/PieceSafari.hpp"

PieceSafari::PieceSafari(const enum Categorie cat, const int posX,
                         const int posY, const int pos2X, const int pos2Y)
    : Piece(to_string(cat), posX, posY), _category(cat), pos2({pos2X, pos2Y}) {}

PieceSafari::~PieceSafari() {}

const std::string PieceSafari::to_string(const enum Categorie cat) const {
  switch (cat) {
  case Barriere:
    return "Barrière";
  case Elephant:
    return "Éléphant";
  case Rhinoceros:
    return "Rhinocéros";
  case Lion:
    return "Lion";

  default:
    throw std::logic_error("Catégorie inconnue (PieceSafari).");
  }
}

const sf::Color PieceSafari::getScreenColor() const {
  // Pas utilisé dans le Safari car les pièces sont différentes par leur formes
  // et leur couleur est donc fixe et défini également dans l'affichage
  return sf::Color::White;
}

enum PieceSafari::Categorie PieceSafari::getCategory() const {
  return _category;
}

std::string PieceSafari::getName() const {
  return categorie;
}

Position PieceSafari::getPos2() const {
  if (_category != Barriere) {
    throw std::logic_error("Cette pièce n'a pas de seconde position.");
  }

  return pos2;
}
