#include "../includes/Joueur.hpp"

#include <algorithm>

Joueur::Joueur(const int num) : id(num) {}

Joueur::~Joueur() {}

std::ostream &operator<<(std::ostream &out, const Joueur &data) {
  out << "Joueur " << data.id << "\npieces: ";

  if (data.pieces.empty()) {
    out << "[]";
  } else {
    out << "[\n";
    for (const Piece *p : data.pieces) {
      out << "  " << *p << ",\n";
    }
    out << ']';
  }

  return out;
}

void Joueur::ajoutPiece(Piece *piece) {
  pieces.push_back(piece);
}

const std::vector<Piece *> Joueur::getPieces() const {
  return pieces;
}

int Joueur::getNum() const {
  return id;
}

bool Joueur::retirePiece(Piece *piece) {
  const auto it = std::find(pieces.begin(), pieces.end(), piece);
  const bool found = it != pieces.end();

  if (found) {
    pieces.erase(it);
  }

  return found;
}
