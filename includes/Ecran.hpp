#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Ecran final {
  static const uint bottomTxtPadding = 30;

  static std::string message;

  static bool fps;

  static int count;

public:
  // Fenêtre
  static sf::RenderWindow window;

  Ecran(const uint width = 600, const uint height = 600,
        const std::string name = "Projet",
        const bool printFps = false); // constructor
  ~Ecran();                           // destructor

  void afficher(
      /* Fonction appellée à chaque image */
      const std::function<void()> drawEachFrame = {[]() {}},

      /* Prend 2 arguments: position X, position Y
       * -> appellé à chaque fois que l'utilisateur fait clic gauche */
      const std::function<void(const int, const int)> doOnLeftClick = {
          [](const int, const int) {}}) const;

  // Largeur fenêtre
  static uint largeur() {
    return window.getSize().x;
  }

  // Hauteur fenêtre
  static uint hauteur() {
    return window.getSize().y - bottomTxtPadding;
  }

  // Ecrire un message en bas de l'écran
  static void printMessage(std::string msg) {
    message = msg;
  }

  // Efface le message en bas de l'écran
  static void cleanMessage() {
    message = "";
  }

  // Change l'état de l'affichage des FPS
  static void toggleFps() {
    fps = !fps;
  }
};
