#include "../../includes/Safari/Safari.hpp"
#include "../../includes/Ecran.hpp"

Safari::Safari(Joueur &j1, Joueur &j2, Joueur *j3)
    : Jeu(j1), plateau(PlateauSafari()), joueur2{j2}, joueur3{j3},
      deplacerAnimal(false), placerBarriere(false), joueursEnVie(3),
      barrieres(0) {
  if (joueur3 == nullptr) {
    --joueursEnVie;
  }
  Ecran::printMessage(msgChoixAnimal());
  etape = ChoixJ1;
}

Safari::~Safari() {}

std::ostream &operator<<(std::ostream &out, const Safari &data) {
  out << "j1: " << data.joueur1 << "\nj2: " << data.joueur2;
  if (data.joueur3 != nullptr) {
    out << "\nj3: " << data.joueur3;
  }

  out << "\nPlateau:\n" << data.plateau;

  return out;
}

PieceSafari *Safari::getPiece(const int x, const int y) const {
  return dynamic_cast<PieceSafari *>(plateau.getPiece(x, y));
}

const PieceSafari *Safari::getPiece(Piece *piece) const {
  return dynamic_cast<const PieceSafari *>(piece);
}

const std::string Safari::msgChoixAnimal() const {
  return "Joueur " + std::to_string(joueurCourant->getNum()) +
         ", choississez un animal.";
}

const std::string Safari::msgPlacement() const {
  return "Joueur " + std::to_string(joueurCourant->getNum()) +
         ", placez votre " +
         getPiece(joueurCourant->getPieces().at(0))->getName() + ".";
}

const std::string Safari::msgTonTour() const {
  std::string action;
  if (!deplacerAnimal) {
    action =
        "déplacez un " + getPiece(joueurCourant->getPieces().at(0))->getName();
  } else {
    action = "placez une barrière en cliquant sur 2 cases";
  }

  return "Joueur " + std::to_string(joueurCourant->getNum()) + ", " + action +
         ".";
}

void Safari::play() {
  plateau.afficherPlateau(std::cout);

  // Demander à J1 de choisir un animal
  if (etape <= Etape::ChoixJ1) {
    return;
  }

  // Demander à J2 de choisir un animal
  if (etape <= Etape::ChoixJ2) {
    return;
  }

  // Demander à J3 de choisir un animal
  if (etape <= Etape::ChoixJ3) {
    if (joueur3 == nullptr) {
      // Pas de joueur 3
      // On retire le dernier choix d'animal
      const Piece *p = plateau.getPieces().at(0);
      Position pos = p->getPos();
      plateau.modifierPlateau(pos.first, pos.second, nullptr);
      delete p;

      // On passe à l'étape suivante
      etape = Etape::Placement;
      Ecran::printMessage(msgPlacement());
    }
    return;
  }

  // Placement des animaux
  if (etape <= Etape::Placement) {
    return;
  }
}

void Safari::choixJoueur(const Etape next) {
  PieceSafari *p = getPiece(posCurseur.first, posCurseur.second);
  // Vérifie qu'une pièce à été cliquée
  if (p == nullptr) {
    return;
  }

  // Donne la pièce au joueur, il est maintenant lié à cette animal
  joueurCourant->ajoutPiece(p);

  // Ajoute les 2 autre animaux que le joueur va devoir placer
  for (int i = 0; i < 2; ++i) {
    joueurCourant->ajoutPiece(new PieceSafari(p->getCategory(), -1, -1));
  }

  // Retire la pièce du plateau
  plateau.modifierPlateau(posCurseur.first, posCurseur.second, nullptr);
  p->moveTo(-1, -1);

  // On passe à l'étape suivante
  etape = next;
  changerJoueurCourant();
  Ecran::printMessage(msgChoixAnimal());
}

void Safari::event(const int x, const int y) {
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
  // Choix animal J1
  case ChoixJ1: {
    choixJoueur(ChoixJ2);
    break;
  }

  // Choix animal J2
  case ChoixJ2: {
    choixJoueur(ChoixJ3);
    break;
  }

  // Choix animal J3
  case ChoixJ3: {
    choixJoueur(Placement);
    Ecran::printMessage(msgPlacement());
    break;
  }

  // Placement des animaux
  case Placement: {
    const Piece *p = getPiece(posCurseur.first, posCurseur.second);
    // Vérifie que la case choisie est vide
    if (p == nullptr) {
      // Placement des animaux des joueurs, tour à tour
      for (Piece *it : joueurCourant->getPieces()) {
        if (it->getPos().first == -1) {
          plateau.modifierPlateau(posCurseur.first, posCurseur.second, it);
          break;
        }
      }

      // On laisse la main
      changerJoueurCourant();

      // Détection tout les joueurs ont placés leurs animaux
      int total = plateau.getPieces().size();
      if ((joueur3 && total == 9) || (!joueur3 && total == 6)) {
        etape = Etape::EnJeu;
        Ecran::printMessage(msgTonTour());
      } else {
        Ecran::printMessage(msgPlacement());
      }
    }

    break;
  }

  case EnJeu: {
    const Piece *p = getPiece(posCurseur.first, posCurseur.second);

    // Déplacer un animal
    if (!deplacerAnimal && p == nullptr) {
      // Vérifier que le déplacement est correct
      if (plateau.deplacementValide(posCurseur.first, posCurseur.second)) {
        plateau.moveSelection(posCurseur.first, posCurseur.second);
        deplacerAnimal = true;
        Ecran::printMessage(msgTonTour());
      }

      break;
    }

    // Poser une barrière
    if (deplacerAnimal && !placerBarriere) {
      PositionMutable placeholder = plateau.getPlaceholderBarriere();
      if (placeholder.first == -1) {
        // On place le placeholder
        plateau.setPlaceholderBarriere(posCurseur.first, posCurseur.second);

      } else {
        // On vérifie que le second point est bien positionné
        // Et on place la barrière si possible
        bool valid;
        if (plateau.validWithPlaceholder(posCurseur.first, posCurseur.second) &&
            (valid = plateau.placementBarriere(posCurseur.first,
                                               posCurseur.second))) {
          plateau.setPlaceholderBarriere(-1, -1);
          placerBarriere = true;
        } else {
          // On déplace le placeholder
          plateau.setPlaceholderBarriere(posCurseur.first, posCurseur.second);
        }
      }
    }

    if (!deplacerAnimal && p != nullptr) {
      // Vérifie que la pièce est à nous
      if (getPiece(posCurseur.first, posCurseur.second)->getCategory() ==
          getPiece(joueurCourant->getPieces().at(0))->getCategory()) {
        // On sélectionne la pièce
        plateau.modifierSelection(posCurseur.first, posCurseur.second);
      }
    }

    // Vérifie si le joueur pourra encore jouer
    // Càd au moins 1 animal possède une zone de déplacement d'au moins 8 cases
    if (plateau.zoneDeplacementOk(joueurCourant) == 0) {
      --joueursEnVie;
    }

    if (deplacerAnimal && placerBarriere) {
      // Vérification partie terminée
      if (joueursEnVie <= 1) {
        etape = Fini;
        // TODO: Afficher le joueur gagnant (s'il y en a un)
        std::string gagnant;
        if (joueursEnVie == 0) {
          gagnant = "Aucun gagnant.";
        } else {
          if (plateau.zoneDeplacementOk(&joueur1) > 0) {
            gagnant = "Joueur 1 (" +
                      getPiece(joueur1.getPieces().at(0))->getName() +
                      ") à gagner";
          } else if (plateau.zoneDeplacementOk(&joueur2) > 0) {
            gagnant = "Joueur 2 (" +
                      getPiece(joueur2.getPieces().at(0))->getName() +
                      ") à gagner";
          } else {
            gagnant = "Joueur 3 (" +
                      getPiece(joueur3->getPieces().at(0))->getName() +
                      ") à gagner";
          }
        }
        Ecran::printMessage("Partie terminée ! " + gagnant);
      } else {
        // Le joueur a fini son tour
        // Réinitialise les variables
        deplacerAnimal = false;
        placerBarriere = false;

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

const Position Safari::getPosition() const {
  if (posCurseur.second > plateau.getTaille() - 1) {
    std::cerr << "Position inconnu du plateau.\n";
    return emptyPosition();
  }

  return posCurseur;
}

void Safari::changerJoueurCourant() {
  if (joueurCourant->getNum() == joueur1.getNum()) {
    joueurCourant = &joueur2;
  } else if (joueurCourant->getNum() == joueur2.getNum()) {
    if (joueur3) {
      joueurCourant = joueur3;
    } else {
      joueurCourant = &joueur1;
    }
  } else {
    joueurCourant = &joueur1;
  }
}
