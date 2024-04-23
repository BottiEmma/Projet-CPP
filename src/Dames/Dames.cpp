#include "../../includes/Dames/Dames.hpp"
#include "../../includes/Dames/PieceDames.hpp"
#include "../../includes/Ecran.hpp"

Dames::Dames(Joueur &j1, Joueur &j2)
    : Jeu(j1), plateau(PlateauDames(j1, j2)), joueur2(j2) {
  std::srand(static_cast<unsigned int>(time(0)));
  joueurCourant = &j2;
  etape = EnJeu;

  Ecran::printMessage("Joueur " + std::to_string(joueurCourant->getNum()) +
                      " commence avec les blancs.");
}

Dames::~Dames() {}

std::ostream &operator<<(std::ostream &out, const Dames &data) {
  out << "j1: " << data.joueur1 << "\nj2: " << data.joueur2 << "\nPlateau:\n"
      << data.plateau;
  return out;
}

const PieceDames *Dames::getPiece(const int x, const int y) const {
  return dynamic_cast<const PieceDames *>(plateau.getPiece(x, y));
}

const PieceDames *Dames::getPiece(Piece *piece) const {
  return dynamic_cast<const PieceDames *>(piece);
}

const std::string Dames::msgTonTour() const {
  return "Joueur " + std::to_string(joueurCourant->getNum()) +
         ", c'est votre tour. ";
}

void Dames::play() {
  plateau.afficherPlateau(std::cout);
}

void Dames::event(const int x, const int y) {
  // Update position
  posCurseur = plateau.trouveCoordonnees(x, y);
  posCurseur = getPosition();
  if (posCurseur.first == -1) {
    // Mauvaise position
    return;
  }

  // Texte de debug
  std::cout << "Clic souris @ (" << x << ", " << y << ") aka ("
            << posCurseur.first << ", " << posCurseur.second << ")\n";

  switch (etape) {
  case EnJeu: {
    const PieceDames *p = getPiece(posCurseur.first, posCurseur.second);
    if (p && p->getCategory() ==
                 getPiece(joueurCourant->getPieces().at(0))->getCategory()) {
      // On sélectionne la pièce
      plateau.modifierSelection(posCurseur.first, posCurseur.second);
    }
    // Si une prise est possible, le joueur est obligé d'en faire une
    if (prisePossible(*joueurCourant)) {
      Ecran::printMessage("Vous devez forcément faire une prise.");
      if (plateau.priseValide(posCurseur.first, posCurseur.second)) {
        if (p == nullptr) {
          // On efface la piece prise du plateau
          Piece *pieceEffacee =
              plateau.piecePrise(posCurseur.first, posCurseur.second);
          if (!pieceEffacee) {
            // Problème : Mauvais déplacement
            break;
          }
          plateau.modifierPlateau(pieceEffacee->getPos().first,
                                  pieceEffacee->getPos().second, nullptr);
          plateau.moveSelection(posCurseur.first, posCurseur.second);
          // Donne la main au joueur suivant
          changerJoueurCourant();
          Ecran::printMessage(msgTonTour());
        }
      }
      break;
      // Quand il ne peux pas faire de prise, il déplace une de ses pieces
    }
    // Vérifier que le déplacement est correct
    if (plateau.deplacementValide(posCurseur.first, posCurseur.second)) {
      // Test si le déplacement est possible pour un pions : en diagonal en
      // direction de l'autre joueur
      if (p == nullptr) {
        plateau.moveSelection(posCurseur.first, posCurseur.second);
        // Donne la main au joueur suivant
        changerJoueurCourant();
        Ecran::printMessage(msgTonTour());
      }
    }
    break;
  }
  case Fini: {
    break;
  }
  }
}

const Position Dames::getPosition() const {
  if (posCurseur.second > plateau.getTaille() - 1) {
    std::cerr << "Position inconnu du plateau.\n";
    return emptyPosition();
  }

  return posCurseur;
}

void Dames::changerJoueurCourant() {
  if (joueurCourant->getNum() == joueur1.getNum()) {
    joueurCourant = &joueur2;
  } else {
    joueurCourant = &joueur1;
  }
}

// TODO: A continuer
// Pas très sûre de comment procéder pour cette fonction : il faudrait vérifier
// les cases en diagnonale des pièces observées mais est-ce qu'il y a besoin que
// les pièces aient des attributs pour leur position (comme proposé dans Piece)
// ?
// UPDATE : utiliser deplacementValide ?
bool Dames::prisePossible(Joueur &joueur) const {
  const std::vector<Piece *> &pieces = joueur.getPieces();
  const PieceDames *pieceAPrendre;
  for (uint i = 0; i < pieces.size(); i++) {
    const PieceDames *p = getPiece(pieces[i]);
    const Position pos = p->getPos();
    if (!p) {
      throw std::runtime_error("Cette pièce est.. étrange.");
    }
    // On regarde si une prise est possible pour chaque coté de la piece
    if (pos.first > 1) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first - 1, pos.second));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first - 2, pos.second)) == nullptr) {
        return true;
      }
    }
    if (pos.first > 1 && pos.second > 1) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first - 1, pos.second - 1));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first - 2, pos.second - 2)) ==
              nullptr) {
        return true;
      }
    }
    if (pos.first > 1 && pos.second < 8) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first - 1, pos.second + 1));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first - 2, pos.second + 2)) ==
              nullptr) {
        return true;
      }
    }
    if (pos.second > 1) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first, pos.second - 1));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first, pos.second - 2)) == nullptr) {
        return true;
      }
    }
    if (pos.second < 8) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first, pos.second + 1));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first, pos.second + 2)) == nullptr) {
        return true;
      }
    }
    if (pos.first < 8 && pos.second > 1) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first + 1, pos.second - 1));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first + 2, pos.second - 2)) ==
              nullptr) {
        return true;
      }
    }
    if (pos.first < 8 && pos.second < 8) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first + 1, pos.second + 1));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first + 2, pos.second + 2)) ==
              nullptr) {
        return true;
      }
    }
    if (pos.first < 8) {
      pieceAPrendre = getPiece(plateau.getPiece(pos.first + 1, pos.second));
      if (pieceAPrendre != nullptr &&
          pieceAPrendre->getCategory() != p->getCategory() &&
          getPiece(plateau.getPiece(pos.first + 2, pos.second)) == nullptr) {
        return true;
      }
    }
    // Cas où la piece est une dame...
    if (!p->getDame()) {
    }
  }
  return false;
}
