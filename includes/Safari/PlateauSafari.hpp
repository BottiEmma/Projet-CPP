#pragma once

#include "../Joueur.hpp"
#include "../Plateau.hpp"
#include "PieceSafari.hpp"

struct PlateauSafari final : public Plateau {
  PlateauSafari();
  virtual ~PlateauSafari();

  // Fonction pour afficher le plateau vers une sortie
  void afficherPlateau(std::ostream &, const bool debug = false) const override;

  // Pareil que deplacementValide mais est utilisable avec des coordonnées
  bool deplacementValideCoor(const int x1, const int y1, const int x2,
                             const int y2) const;

  // Vérifie que le déplacement est valide
  bool deplacementValide(const int destX, const int destY) const;

  // Renvoie la position du placeholder
  Position getPlaceholderBarriere() const;

  // Modifie la position du placeholder
  void setPlaceholderBarriere(const int x, const int y);

  // Avec les positions du placeholder et les positions données
  // Vérifie que les 2 points sont à côtés
  bool validWithPlaceholder(const int x, const int y) const;

  // Place la barrière, renvoie si on a réussi
  bool placementBarriere(const int x, const int y);

  // Renvoie le nombre d'animaux jouable pour un joueur donnné
  int zoneDeplacementOk(const Joueur *joueur) const;

private:
  // Barrières
  std::vector<PieceSafari *> barrieres;

  PositionMutable placeholderBarriere;
};
