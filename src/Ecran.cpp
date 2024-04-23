#include "../includes/Ecran.hpp"

#include <iomanip>
#include <iostream>

using namespace sf;

RenderWindow Ecran::window;

std::string Ecran::message = "";

bool Ecran::fps = false;

int Ecran::count = 0;

Ecran::Ecran(const uint w, const uint h, const std::string n, const bool pf) {
  if (count >= 1) {
    throw std::logic_error("Uniquement 1 écran supporté.");
  }
  ++count;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  // Création de la fenêtre SFML
  window.create(VideoMode(w, h + bottomTxtPadding), n,
                Style::Titlebar | Style::Close, settings);

  fps = pf;
}

Ecran::~Ecran() {
  --count;
}

void Ecran::afficher(
    const std::function<void()> dessin,
    const std::function<void(const int, const int)> onLeftClick) const {
  Font font;
  font.loadFromFile("assets/open-sans-latin-400-normal.ttf");

  Text text;
  text.setFont(font);
  text.setCharacterSize(24);
  text.setPosition(3, hauteur());
  text.setFillColor(Color::White);

  Clock fpsClock;
  Clock printFpsclock;

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      // Fermeture de la fenêtre
      if (event.type == Event::Closed) {
        window.close();
      }

      // Récupération des coordonnées du clic de souris
      if (event.type == Event::MouseButtonPressed &&
          event.mouseButton.button == Mouse::Left) {
        onLeftClick(event.mouseButton.x, event.mouseButton.y);
      }
    }

    window.clear();

    // Appel d'une fonction dessin
    dessin();

    // Ecriture du message
    text.setString(String::fromUtf8(message.begin(), message.end()));
    window.draw(text);

    window.display();

    // Affichage des FPS
    if (fps) {
      const float currentTime = fpsClock.restart().asSeconds();

      // Toutes les 2 secondes
      if (printFpsclock.getElapsedTime().asSeconds() >= 2.f) {
        std::cout << "  " << std::fixed << std::setprecision(2)
                  << 1.f / currentTime << std::endl;
        printFpsclock.restart();
      }
    }
  }
}
