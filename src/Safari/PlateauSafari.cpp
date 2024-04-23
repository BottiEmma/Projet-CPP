#include "../../includes/Safari/PlateauSafari.hpp"
#include "../../includes/Ecran.hpp"

PlateauSafari::PlateauSafari()
    : Plateau(8), placeholderBarriere(emptyPosition()) {
  // Au début, le plateau est vide

  // Les joueurs vont choisir leur animal, on place autant d'animal différent
  // que de joueur différent sur le plateau, au centre parce que les joueurs
  // vont cliquer dessus pour faire leur choix
  plateau[2][3] = new PieceSafari(PieceSafari::Elephant, 2, 3);
  plateau[3][3] = new PieceSafari(PieceSafari::Lion, 3, 3);
  plateau[4][3] = new PieceSafari(PieceSafari::Rhinoceros, 4, 3);
}

PlateauSafari::~PlateauSafari() {}

void PlateauSafari::afficherPlateau(std::ostream &out, const bool d) const {
  const float tailleCellule = static_cast<float>(Ecran::largeur()) / taille;

  // Cellule
  sf::RectangleShape cell(sf::Vector2f(tailleCellule, tailleCellule));

  // Couleurs
  sf::Color jaune = sf::Color(255, 200, 40);
  sf::Color marron = sf::Color(155, 60, 40);

  // Pièces
  sf::CircleShape rhinoceros(tailleCellule / 3, 3);
  rhinoceros.setOutlineThickness(2.);
  sf::CircleShape elephant(tailleCellule / 3, 6);
  elephant.setOutlineThickness(2.);
  sf::CircleShape lion(tailleCellule / 3);
  lion.setOutlineThickness(2.);
  sf::RectangleShape barriereH(sf::Vector2f(tailleCellule, tailleCellule / 6));
  barriereH.setFillColor(sf::Color::Black);
  sf::RectangleShape barriereV(sf::Vector2f(tailleCellule / 6, tailleCellule));
  barriereV.setFillColor(sf::Color::Black);

  sf::CircleShape placeholder(tailleCellule / 5);
  placeholder.setFillColor(sf::Color::Magenta);

  const float decalagePiece = tailleCellule / 6;

  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < taille; j++) {
      // Remet la couleur de l'outline par défaut
      rhinoceros.setOutlineColor(sf::Color::Transparent);
      elephant.setOutlineColor(sf::Color::Transparent);
      lion.setOutlineColor(sf::Color::Black);

      const float x = i * tailleCellule;
      const float y = j * tailleCellule;

      // Position de la cellule et de la pièce
      cell.setPosition(x, y);
      rhinoceros.setPosition(x + decalagePiece, y + decalagePiece);
      elephant.setPosition(x + decalagePiece, y + decalagePiece);
      lion.setPosition(x + decalagePiece, y + decalagePiece);
      if (d) {
        out << "(" << x << ", " << y;
      }

      // Alternation des couleurs
      if ((i + j) % 2 == 0) {
        cell.setFillColor(jaune);
        if (d) {
          out << ", J), ";
        }
      } else {
        cell.setFillColor(marron);
        if (d) {
          out << ", M), ";
        }
      }

      // Dessine la cellule
      Ecran::window.draw(cell);

      // Dessine la piece
      const Piece *p = plateau[i][j];
      if (p != nullptr) {
        if (p->isSelectionnee()) {
          lion.setOutlineColor(sf::Color::Green);
          rhinoceros.setOutlineColor(sf::Color::Green);
          elephant.setOutlineColor(sf::Color::Green);
        }
        switch (dynamic_cast<const PieceSafari *>(p)->getCategory()) {
        case PieceSafari::Elephant: {
          elephant.setFillColor(sf::Color::White);
          Ecran::window.draw(elephant);
          break;
        }
        case PieceSafari::Rhinoceros: {
          rhinoceros.setFillColor(sf::Color::Red);
          Ecran::window.draw(rhinoceros);
          break;
        }
        case PieceSafari::Lion: {
          lion.setFillColor(sf::Color::Yellow);
          Ecran::window.draw(lion);
          break;
        }
        default:
          break;
        }
      }
    }
    if (d) {
      out << "\n";
    }
  }

  // Dessine les mur
  for (PieceSafari *it : barrieres) {
    Position pos1 = it->getPos();
    Position pos2 = it->getPos2();

    const float decalage = 5;
    // Horizontale
    if (pos1.first == pos2.first) {
      const int x = pos1.first;
      const int y = std::max(pos1.second, pos2.second);
      barriereH.setPosition(x * tailleCellule, y * tailleCellule - decalage);
      Ecran::window.draw(barriereH);
    }

    // Verticale
    else {
      const int x = std::max(pos1.first, pos2.first);
      const int y = pos1.second;
      barriereV.setPosition(x * tailleCellule - decalage, y * tailleCellule);
      Ecran::window.draw(barriereV);
    }
  }

  // Dessinne les indications pour le placement des murs
  if (placeholderBarriere.first != -1) {
    const int decalagePlaceholder = decalagePiece + 10;
    placeholder.setPosition(
        placeholderBarriere.first * tailleCellule + decalagePlaceholder,
        placeholderBarriere.second * tailleCellule + decalagePlaceholder);
    Ecran::window.draw(placeholder);
  }

  if (d) {
    out << "---\n";
  }
}

bool PlateauSafari::deplacementValideCoor(const int x1, const int y1,
                                          const int x2, const int y2) const {
  // Vérifier qu'il n'y a pas de barrière sur le chemin
  for (PieceSafari *barriere : barrieres) {
    Position pos1 = barriere->getPos();
    Position pos2 = barriere->getPos2();

    /* Conditions si vrai = bloqués = une barrière est sur le chemin
     * Ordre dans les vérifications :
     * - Horizontal/Vertical
     * - Sens du déplacement
     * - Barrière horizontal/vertical
     * - Barrière sur la ligne/colonne
     * - Départ avant la barrière
     * - Arrivé après la barrière */
    const bool deplacementVertical = x1 == x2;
    const bool deplacementHorizontal = y1 == y2;

    const bool versGauche = x1 > x2;
    const bool versDroite = x1 < x2;
    const bool monter = y1 > y2;
    const bool descendre = y1 < y2;

    const bool barriereVertical = pos1.second == pos2.second &&
                                  // Bien positionnée
                                  y1 == pos1.second;
    const bool barriereHorizontal = pos1.first == pos2.first &&
                                    // Bien positionnée
                                    x1 == pos1.first;

    // Gauche vers droite
    const bool gvd = deplacementHorizontal && barriereVertical && versDroite &&
                     // Départ avant barrière et arrivé après
                     x1 < std::max(pos1.first, pos2.first) &&
                     x2 >= std::max(pos1.first, pos2.first);
    // Droite vers gauche
    const bool dvg = deplacementHorizontal && barriereVertical && versGauche &&
                     // Départ avant barrière et arrivé après
                     x1 > std::min(pos1.first, pos2.first) &&
                     x2 <= std::min(pos1.first, pos2.first);
    // Haut vers bas / Descendre
    const bool hvb = deplacementVertical && barriereHorizontal && descendre &&
                     // Départ avant barrière et arrivé après
                     y1 < std::max(pos1.second, pos2.second) &&
                     y2 >= std::max(pos1.second, pos2.second);
    // Bas vers haut / Monter
    const bool bvh = deplacementVertical && barriereHorizontal && monter &&
                     // Départ avant barrière et arrivé après
                     y1 > std::min(pos1.second, pos2.second) &&
                     y2 <= std::min(pos1.second, pos2.second);

    if (gvd || dvg || hvb || bvh) {
      return false;
    }
  }

  return (x2 == x1 || y2 == y1);
}

bool PlateauSafari::deplacementValide(const int x, const int y) const {
  if (!selection) {
    return false;
  }

  Position posInitiale = selection->getPos();

  return deplacementValideCoor(posInitiale.first, posInitiale.second, x, y);
}

Position PlateauSafari::getPlaceholderBarriere() const {
  return placeholderBarriere;
}

void PlateauSafari::setPlaceholderBarriere(const int x, const int y) {
  placeholderBarriere = {x, y};
}

bool PlateauSafari::validWithPlaceholder(const int x, const int y) const {
  const int diffX = std::abs(placeholderBarriere.first - x);
  const int diffY = std::abs(placeholderBarriere.second - y);

  // Vérification adjacence
  return (diffX == 1 && diffY == 0) || (diffX == 0 && diffY == 1);
}

bool PlateauSafari::placementBarriere(const int x, const int y) {
  // Vérifie qu'il n'y a pas déjà une pièce ici
  for (PieceSafari *it : barrieres) {
    Position pos1 = it->getPos();
    Position pos2 = it->getPos2();
    if ((x == pos1.first && y == pos1.second) ||
        (x == pos2.first && y == pos2.second)) {
      // Le point existe déjà
      return false;
    }
  }

  barrieres.push_back(new PieceSafari(PieceSafari::Barriere,
                                      placeholderBarriere.first,
                                      placeholderBarriere.second, x, y));

  return true;
}

int PlateauSafari::zoneDeplacementOk(const Joueur *joueur) const {
  // Compte le nombre d'animaux qui ont encore des zones valides
  // Zone valide = animal possède une zone de déplacement de mini 8 cases

  // Nombre d'animaux dans une zone libre valide
  int nbAnimaux = 0;

  for (const Piece *animal : joueur->getPieces()) {
    Position position = animal->getPos();

    // Nombre de cases sur lesquels l'animal peut se rendre
    int casesDisponibles = 0;

    bool cherchesCasesDispo = true;
    for (int i = 0; i < taille && cherchesCasesDispo; ++i) {
      for (int j = 0; j < taille && cherchesCasesDispo; ++j) {
        // Regarde les cases vide uniquement
        if (plateau[i][j] != nullptr) {
          continue;
        }

        // Case dispo si on peut se déplacer dessus
        if (deplacementValideCoor(position.first, position.second, i, j)) {
          ++casesDisponibles;
        }

        // Si 8 cases sont dispo on peut arrêter la recherche pour cette animal
        if (casesDisponibles >= 8) {
          ++nbAnimaux;
          cherchesCasesDispo = false;
        }
      }
    }
  }

  return nbAnimaux;
}
