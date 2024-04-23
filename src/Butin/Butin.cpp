#include "../../includes/Butin/Butin.hpp"
#include "../../includes/Butin/PieceButin.hpp"
#include "../../includes/Ecran.hpp"

Butin::Butin(Joueur &j1, Joueur &j2)
    : Jeu(j1), plateau(PlateauButin()), joueur2{j2} {
  // Demander à J1 de retirer une pièce jaune
  Ecran::printMessage(msgPieceJaune());
  etape = ChoixJ1;
}

Butin::~Butin() {}

std::ostream &operator<<(std::ostream &out, const Butin &data) {
  out << "j1: " << data.joueur1 << "\nj2: " << data.joueur2 << "\nPlateau:\n"
      << data.plateau;
  return out;
}

const PieceButin *Butin::getPiece(const int x, const int y) const {
  return dynamic_cast<const PieceButin *>(plateau.getPiece(x, y));
}

const std::string Butin::msgPieceJaune(const bool erreur) const {
  std::string res = "Joueur " + std::to_string(joueurCourant->getNum()) +
                    ", retirez une piece jaune en cliquant dessus.";

  if (erreur) {
    res = "Mauvaise piece. " + res;
  }

  return res;
}

const std::string Butin::msgTonTour() const {
  return "Joueur " + std::to_string(joueurCourant->getNum()) +
         ", c'est votre tour. " + msgPoints(calculpoints());
}

const std::string Butin::msgPoints(Position points) const {
  return "J1 : " + std::to_string(points.first) + " vs " +
         std::to_string(points.second) + " : J2";
}

void Butin::play() {
  plateau.afficherPlateau(std::cout);
  if (etape <= Etape::ChoixJ1) {
    return;
  }

  // Demander à J2 de retirer une pièce jaune
  if (etape <= Etape::ChoixJ2) {
    return;
  }
}

void Butin::event(const int x, const int y) {
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
  // Clic du J1
  case (Etape::ChoixJ1): {
    const PieceButin *p = getPiece(posCurseur.first, posCurseur.second);
    // Vérifier que la pièce est jaune
    if (p->getPoints() == PieceButin::Jaune) {

      // Retire la pièce
      plateau.modifierPlateau(posCurseur.first, posCurseur.second, nullptr);
      delete p;

      // On passe à l'étape suivante
      etape = ChoixJ2;
      changerJoueurCourant();
      Ecran::printMessage(msgPieceJaune());
    } else {
      Ecran::printMessage(msgPieceJaune(true));
    }
    break;
  }

  // Clic du J2
  case ChoixJ2: {
    // Vérifier que la pièce existe et est jaune
    const PieceButin *p = getPiece(posCurseur.first, posCurseur.second);
    if (p && p->getPoints() == PieceButin::Jaune) {
      // Retire la pièce
      plateau.modifierPlateau(posCurseur.first, posCurseur.second, nullptr);
      delete p;

      // On passe à l'étape suivante
      etape = EnJeu;
      changerJoueurCourant();
      Ecran::printMessage(msgTonTour());
    } else {
      Ecran::printMessage(msgPieceJaune(true));
    }
    break;
  }
  case EnJeu: {
    const Piece *p = getPiece(posCurseur.first, posCurseur.second);
    // Vérifie que la pièce sélectionnée = Jaune et que la case choisie = vide
    if (plateau.selectionJaune() && p == nullptr) {
      // Récupère la liste des pièces sur le chemin
      const std::vector<const Piece *> chemin =
          plateau.cheminPieces(posCurseur.first, posCurseur.second);

      // Si le chemin est valide == Si des pièces sont sur le chemin
      if (!chemin.empty()) {
        // Déplace la pièce vers l'arrivé
        plateau.moveSelection(posCurseur.first, posCurseur.second);

        // Mange les pièces sur le chemin
        for (const Piece *it : chemin) {
          // Récupère la pièce
          const Position pos = it->getPos();
          Piece *gain = plateau.getPiece(pos.first, pos.second);

          // Retire la pièce
          plateau.modifierPlateau(pos.first, pos.second, nullptr);

          // Ajoute le point au joueur
          joueurCourant->ajoutPiece(gain);
        }

        // Vérification partie terminé = si plus aucun coup n'est possible
        if (!plateau.coupsPossible()) {
          etape = Fini;
          Ecran::printMessage("Partie terminée ! " +
                              msgPoints(calculpoints(true)));
        } else {
          // On donne la main à l'adversaire
          changerJoueurCourant();
          Ecran::printMessage(msgTonTour());
        }
      }
    }

    // Sélectionne la pièce
    else {
      plateau.modifierSelection(posCurseur.first, posCurseur.second);
    }

    break;
  }
  case Fini: {
    break;
  }
  }
}

const Position Butin::getPosition() const {
  if (posCurseur.second > plateau.getTaille() - 1) {
    std::cerr << "Position inconnu du plateau.\n";
    return emptyPosition();
  }

  return posCurseur;
}

void Butin::changerJoueurCourant() {
  if (joueurCourant->getNum() == joueur1.getNum()) {
    joueurCourant = &joueur2;
  } else {
    joueurCourant = &joueur1;
  }
}

Position Butin::calculpoints(const bool end) const {
  // Nombre de points des joueurs
  int j1 = 0, j2 = 0;
  for (const Piece *it : joueur1.getPieces()) {
    j1 += dynamic_cast<const PieceButin *>(it)->getPoints();
  }
  for (const Piece *it : joueur2.getPieces()) {
    j2 += dynamic_cast<const PieceButin *>(it)->getPoints();
  }

  // Si c'est le calcul des points de la fin de partie
  if (end) {
    // Nombre de points sur le plateau restant
    int malus = 0;
    for (const Piece *it : plateau.getPieces()) {
      malus += dynamic_cast<const PieceButin *>(it)->getPoints();
    }

    // C'est le joueur qui a joué le dernier coup qui reçoit le malus
    if (joueurCourant->getNum() == joueur1.getNum()) {
      // Joueur 1 reçoit le malus
      j1 -= malus;
      if (j1 < 0) {
        j1 = 0;
      }
    } else {
      // Joueur 2 reçoit le malus
      j2 -= malus;
      if (j2 < 0) {
        j2 = 0;
      }
    }
  }

  return {j1, j2};
}
