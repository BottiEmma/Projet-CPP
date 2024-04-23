#include "../includes/Butin/Butin.hpp"
#include "../includes/Dames/Dames.hpp"
#include "../includes/Ecran.hpp"
#include "../includes/Safari/Safari.hpp"

void help(char const *progName) {
  std::cout << "Menu d'aide de " << progName << "\n";

  std::cout << "\t" << progName << " butin\n";
  std::cout << "\t\t"
            << "Lance le jeu \"Butin\"\n";

  std::cout << "\t" << progName << " dames\n";
  std::cout << "\t\t"
            << "Lance le jeu \"Dames\"\n";

  std::cout << "\t" << progName << " safari\n";
  std::cout << "\t\t"
            << "Lance le jeu \"Safari\"\n";
}

int main(int argc, char const *argv[]) {

  // Interface cli
  if (argc >= 2) {
    Joueur j1(1);

    const std::string arg = argv[1];
    if (arg.compare("help") == 0) {
      help(argv[0]);

      return EXIT_SUCCESS;
    }

    else if (arg.compare("butin") == 0) {
      Joueur j2(2);

      Butin b(j1, j2);

      const Ecran e;
      // e.toggleFps();
      e.afficher({[&]() { b.play(); }},
                 {[&](const int x, const int y) { b.event(x, y); }});
    }

    else if (arg.compare("dames") == 0) {
      Joueur j2(2);

      Dames d(j1, j2);

      const Ecran e;
      // e.toggleFps();
      e.afficher({[&]() { d.play(); }},
                 {[&](const int x, const int y) { d.event(x, y); }});
    }

    else if (arg.compare("safari") == 0) {
      Joueur j2(2);
      Safari *s;
      Joueur *j3 = nullptr;

      int reponse = 3;
      std::cout << "2 ou 3 joueurs ? ";
      std::cin >> reponse;

      if (reponse == 3) {
        j3 = new Joueur(3);
        s = new Safari(j1, j2, j3);
      } else {
        s = new Safari(j1, j2);
      }

      const Ecran e;
      // e.toggleFps();
      e.afficher({[&]() { s->play(); }},
                 {[&](const int x, const int y) { s->event(x, y); }});

      delete s;
      if (j3 != nullptr) {
        delete j3;
      }
    }

    else {
      std::cerr << "Jeu inconnu.\n";
      help(argv[0]);

      return EXIT_FAILURE;
    }

    std::cout << "Merci d'avoir joué !" << std::endl;
  } else {
    // Interface graphique, menu de selection de jeu ?
    help(argv[0]);
  }

  return EXIT_SUCCESS;
}
