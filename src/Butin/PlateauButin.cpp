#include "../../includes/Butin/PlateauButin.hpp"
#include "../../includes/Butin/PieceButin.hpp"

#include <algorithm>
#include <random>

PlateauButin::PlateauButin() : Plateau(8) {
  // Vecteur de toutes les pièces du jeu
  std::vector<PieceButin *> pieces;
  for (int i = 0; i < 34; i++) {
    pieces.push_back(new PieceButin(PieceButin::Jaune, -1, -1));
  }
  for (int i = 0; i < 20; i++) {
    pieces.push_back(new PieceButin(PieceButin::Rouge, -1, -1));
  }
  for (int i = 0; i < 10; i++) {
    pieces.push_back(new PieceButin(PieceButin::Noire, -1, -1));
  }

  // Mélange le vecteur de pièces (j'ai jamais utilisé ça pour randomiser faudra
  // que je test que ça fonctionne bien)
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(pieces.begin(), pieces.end(), g);

  // Place toutes les pieces sur le plateau
  int index = 0;
  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      plateau[i][j] = pieces[static_cast<uint>(index++)];

      // Met à jour la position
      plateau[i][j]->moveTo(i, j);
    }
  }
}

PlateauButin::~PlateauButin() {
  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      delete plateau[i][j];
    }
  }
}

const std::vector<const Piece *>
PlateauButin::cheminPieces(const int destX, const int destY) const {
  const Position posSelection = selection->getPos();
  return cheminPieces(posSelection.first, posSelection.second, destX, destY);
}

const std::vector<const Piece *>
PlateauButin::cheminPieces(const int depX, const int depY, const int destX,
                           const int destY) const {
  // Distances
  const int deltaX = destX - depX;
  const int deltaY = destY - depY;

  // Direction à prendre
  const int stepX = (deltaX > 0) ? 1 : -1;
  const int stepY = (deltaY > 0) ? 1 : -1;

  std::vector<const Piece *> chemin;

  // Déplacement vertical
  if (deltaX == 0) {
    for (int dy = depY + stepY; dy != destY; dy += stepY) {
      chemin.push_back(plateau[depX][dy]);
    }
  }

  // Déplacement horizontal
  else if (deltaY == 0) {
    for (int dx = depX + stepX; dx != destX; dx += stepX) {
      chemin.push_back(plateau[dx][depY]);
    }
  }

  // Déplacement en diagonale
  else if (abs(deltaX) == abs(deltaY)) {
    for (int dx = depX + stepX, dy = depY + stepY; dx != destX;
         dx += stepX, dy += stepY) {
      chemin.push_back(plateau[dx][dy]);
    }
  }

  // Retire les cases vides
  chemin.erase(std::remove(chemin.begin(), chemin.end(), nullptr),
               chemin.end());

  return chemin;
}

bool PlateauButin::selectionJaune() const {
  if (selection) {
    return dynamic_cast<const PieceButin *>(selection)->getPoints() ==
           PieceButin::Jaune;
  }

  return false;
}

bool PlateauButin::coupsPossible() const {
  // Liste des cases vides
  std::vector<Position> casesVides;

  // Liste des pièces jaunes
  std::vector<Position> piecesJaunes;

  for (int i = 0; i < taille; ++i) {
    for (int j = 0; j < taille; ++j) {
      if (plateau[i][j] == nullptr) {
        // Si case vide
        casesVides.push_back({i, j});
      } else {
        // Sinon
        if (dynamic_cast<const PieceButin *>(plateau[i][j])->getPoints() ==
            PieceButin::Jaune) {
          // Si pièce jaune
          piecesJaunes.push_back({i, j});
        }
      }
    }

    for (Position p : piecesJaunes) {
      // Parmis toutes les pièces jaunes
      for (Position it : casesVides) {
        // Si le déplacement vers une case vide peut donner des points
        if (!cheminPieces(p.first, p.second, it.first, it.second).empty()) {
          return true;
        }
      }
    }
  }

  return false;
}
