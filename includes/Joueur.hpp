#pragma once

#include "../includes/Piece.hpp"
#include <vector>

class Joueur final {
  friend std::ostream &operator<<(std::ostream &, const Joueur &);

  // Numéro du joueur
  int id;

  // Pièces du joueur
  // Pour le butin, tout simplement les pièces que le joueur gagne.
  // Pour les dames, peut être uniquement les pieces sur le plateau.
  // Pour le safari, les animaux du joueur
  std::vector<Piece *> pieces;

public:
  Joueur(const int numeroJoueur); // constructor
  virtual ~Joueur();              // destructor

  // Ajoute une pièce à la liste de pièces du joueur
  void ajoutPiece(Piece *piece);

  // Getter pour les pièces du joueur
  const std::vector<Piece *> getPieces() const;

  // Getter pour le nom du joueur
  int getNum() const;

  // Fonction qui supprime une pièce de la liste de pièces du joueur
  bool retirePiece(Piece *piece);
};
