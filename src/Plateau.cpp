#include "../includes/Plateau.hpp"
#include "../includes/Ecran.hpp"

Plateau::Plateau(const int t, const sf::Color b, const sf::Color n)
    : blanc(b), noir(n), plateau(new Piece **[t]), taille(t) {
  // Création du plateau vide
  for (int i = 0; i < t; i++) {
    plateau[i] = new Piece *[t];
    for (int j = 0; j < t; j++) {
      plateau[i][j] = nullptr;
    }
  }
  selection = nullptr;
}

Plateau::~Plateau() {
  for (int i = 0; i < taille; i++) {
    delete[] plateau[i];
  }
  delete[] plateau;
}

std::ostream &operator<<(std::ostream &out, const Plateau &data) {
  data.afficherPlateau(out, false);
  return out;
}

void Plateau::afficherPlateau(std::ostream &out, const bool d) const {
  const float tailleCellule = static_cast<float>(Ecran::largeur()) / taille;

  const float decalagePiece = tailleCellule / 6;

  // Cellule
  sf::RectangleShape cell(sf::Vector2f(tailleCellule, tailleCellule));

  // Pièce
  sf::CircleShape piece(tailleCellule / 3);
  piece.setOutlineThickness(2.);

  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      const float x = i * tailleCellule;
      const float y = j * tailleCellule;

      // Position de la cellule et de la pièce
      cell.setPosition(x, y);
      piece.setPosition(x + decalagePiece, y + decalagePiece);
      if (d) {
        out << "(" << x << ", " << y;
      }

      // Alternation des couleurs
      if ((i + j) % 2 == 0) {
        cell.setFillColor(blanc);
        piece.setOutlineColor(sf::Color::Black);
        if (d) {
          out << ", B), ";
        }
      } else {
        cell.setFillColor(noir);
        piece.setOutlineColor(sf::Color::White);
        if (d) {
          out << ", N), ";
        }
      }

      // Dessine la cellule
      Ecran::window.draw(cell);

      // Dessine la pièce
      const Piece *p = plateau[i][j];
      if (p != nullptr) {
        if (p->isSelectionnee()) {
          piece.setOutlineColor(sf::Color::Green);
        }
        piece.setFillColor(p->getScreenColor());
        Ecran::window.draw(piece);
      }
    }
    if (d) {
      out << "\n";
    }
  }
  if (d) {
    out << "---\n";
  }
}

void Plateau::modifierPlateau(const int x, const int y, Piece *piece) const {
  if (x >= 0 && x < taille && y >= 0 && y < taille) {
    if (plateau[x][y]) {
      plateau[x][y]->moveTo(-1, -1);
    }

    plateau[x][y] = piece;
    if (piece) {
      piece->moveTo(x, y);
    }

  } else {
    throw std::invalid_argument("Coordonnées invalides");
  }
}

Piece *Plateau::getPiece(const int x, const int y) const {
  if (x >= 0 && x < taille && y >= 0 && y < taille) {
    return plateau[x][y];
  } else {
    throw std::invalid_argument("Coordonnées invalides");
  }
}

const Position Plateau::trouveCoordonnees(const int x, const int y) const {
  const float tailleCelluleX = static_cast<float>(Ecran::largeur()) / taille;
  const float tailleCelluleY = static_cast<float>(Ecran::hauteur()) / taille;

  return {x / tailleCelluleX, y / tailleCelluleY};
}

int Plateau::getTaille() const {
  return taille;
}

void Plateau::modifierSelection(const int x, const int y) {
  Piece *p = getPiece(x, y);
  if (p == nullptr) {
    // Si rien est selectionné on ne fait rien
    return;
  }

  if (selection) {
    // Déselectionne l'ancienne sélection
    selection->changeSelection();
  }

  if (p != selection) {
    // Si la sélection à changer alors changer l'état de la nouvelle pièce
    p->changeSelection();
    selection = p;
  } else {
    // Si c'était la même alors on ne sélectionne plus rien
    selection = nullptr;
  }
}

const Position Plateau::moveSelection(const int x, const int y) {
  if (selection == nullptr) {
    // Ne fais rien si on a rien a bouger
    return emptyPosition();
  }

  // Récupère les coordonnées
  const Position ancienneCoordonnees = {selection->x, selection->y};

  // Retire la pièce de là où elle est pour le plateau
  plateau[ancienneCoordonnees.first][ancienneCoordonnees.second] = nullptr;

  // Déplace la pièce sur le plateau
  modifierPlateau(x, y, selection);

  // Met à jour les coordonnées de la pièce
  selection->moveTo(x, y);

  // Déselectionne la pièce
  modifierSelection(x, y);

  return ancienneCoordonnees;
}

std::vector<const Piece *> Plateau::getPieces() const {
  std::vector<const Piece *> pieces;
  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      if (plateau[i][j]) {
        pieces.push_back(plateau[i][j]);
      }
    }
  }

  return pieces;
}
