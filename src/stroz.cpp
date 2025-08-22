#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#include "functions.hpp"
#include "recall.hpp"

namespace hp {
// funzione per verificare se un punto è dentro uno sprite
bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos) {
  return sprite.getGlobalBounds().contains(mousePos);
}

// std::vector<bool> optionSelected(4, false);

int draw_stroz() {
  const float virtualWidth = 1900.f;
  const float virtualHeight = 800.f;

  sf::RenderWindow window(sf::VideoMode(virtualWidth, virtualHeight),
                          "hopfield_network", sf::Style::Close);

  sf::View view(sf::FloatRect(0, 0, virtualWidth, virtualHeight));
  window.setView(view);

  sf::Font font;
  if (!font.loadFromFile("resources/arial.ttf")) {
    throw std::runtime_error(
        "Error: impossible loading font from resources/arial.ttf");
  }

  bool showNoisedImage = false;

  // Percorsi delle immagini
  std::vector<std::string> file_names = {"gigi.png", "kusozu.png", "noface.png",
                                         "totoro.png"};

  std::string zoomed = "resources/images/zoomed/";
  std::string zoomed_w_noise = "resources/images/zoomed_w_noise/";
  std::string noised = "resources/images/noised/";

  // inizializzo i 4 sprites che andranno in alto (quelli normali)
  std::vector<sf::Texture> textures(4);
  std::vector<sf::Sprite> sprites(4);

  std::vector<std::string> zoomed_w_noisepath(4);
  // percorsi delle versioni corrotte delle immagini

  /*forse qui dovremmo aggiungere un altro
    path per le immagini da dare in input
    a neuron update? (noised, non zoomed_w_noise)*/

  // carica immagini originali
  for (int i = 0; i < 4; ++i) {
    if (!textures[i].loadFromFile(zoomed + file_names[i])) {
      throw std::runtime_error("Error: imposssible loading image from " +
                               zoomed + file_names[i]);
    }
    sprites[i].setTexture(textures[i]);
    sprites[i].setPosition(i * 475.f + 109.5f,
                           80.f);  // distanza tra immagini, le ho centrate
    zoomed_w_noisepath[i] =
        zoomed_w_noise +
        file_names[i];  //<-- prende l'immagine noised e zoomata e la salva
  }

  // inizializzo lo sprite noised, in basso a sx
  sf::Texture texturenoised;
  sf::Sprite spritenoised;
  bool is_noised = false;

  // inizializo l'oggetto recall
  Recall rec("data");
  std::vector<Recall> rec_vec;
  rec_vec.reserve(4);  // creo un vettore di 4 recall
  for (int i = 0; i < 4; ++i) {
    rec_vec.push_back(rec);
  }
  for (int i = 0; i < 4; ++i) {
    rec_vec[i].initialize_from_image(
        zoomed_w_noisepath[i]);  // a ciascun recall affido la gestione di un
                                 // pattern
  }
  bool emanuele = false;

  // inizializzo lo sprite in evoluzione
  sf::Texture texturerecall;
  sf::Sprite spriterecall;
  bool runningrecall = false;

  // inizializzo la scritta per la funzione energia
  /*sf::Text energyText;
  energyText.setFont(font);
  energyText.setCharacterSize(18);
  energyText.setFillColor(sf::Color::White);
  energyText.setPosition(500.f, 1400.f);*/

  // ciclo principale che racchiude tutta la grafica che si vede a schermo
  while (window.isOpen()) {
    sf::Event event;                 // struttura x gestire eventi
    while (window.pollEvent(event))  // prende tutti gli eventi disponibili
    {
      // gestione ridimensionamento (mantiene le proporzioni)
      if (event.type == sf::Event::Resized) {
        float windowRatio =
            static_cast<float>(event.size.width) / event.size.height;
        float viewRatio = virtualWidth / virtualHeight;

        sf::FloatRect viewport;

        if (windowRatio > viewRatio) {
          float scale = viewRatio / windowRatio;
          viewport = {(1.f - scale) / 2.f, 0.f, scale, 1.f};
        } else {
          float scale = windowRatio / viewRatio;
          viewport = {0.f, (1.f - scale) / 2.f, 1.f, scale};
        }

        view.setViewport(viewport);
        window.setView(view);
      }

      if (event.type == sf::Event::Closed)
        window.close();  // chiude la finestra se premo x

      // gestione click
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button ==
              sf::Mouse::Left) {  // premere pulsante sinistro

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(
            window));  // mapPixelToCoords converte posizione del mouse da pixel
                       // a coordinate della vista

        for (int i = 0; i < 4; ++i) {
          if (isSpriteClicked(sprites[i], mousePos)) {
            // Carica immagine corrotta corrispondente
            if (!texturenoised.loadFromFile(zoomed_w_noisepath[i])) {
              throw std::runtime_error("Error: impossible loading image from " +
                                       zoomed_w_noisepath[i]);
            } else {
              spritenoised.setTexture(texturenoised);
              is_noised = true;
            }

            // Mostra immagine (modificata o originale)
            // spritenoised.setTexture(texturenoised);
            spritenoised.setPosition(283.f, 450.f);
            showNoisedImage = true;
          }
        }

        if (isSpriteClicked(spritenoised, mousePos)) {
          // Carica immagine corrotta corrispondente
          texturerecall =
              texturenoised;  // copia la texture corrente della noised
          spriterecall.setTexture(texturerecall);

          // posiziona al centro
          float centerX =
              virtualWidth / 2.f - spriterecall.getGlobalBounds().width / 2.f;
          float posY = spritenoised.getPosition().y;
          spriterecall.setPosition(centerX, posY);

          runningrecall = true;
        }

        for (int i = 0; i < 4; ++i) {
          if (isSpriteClicked(spriterecall, mousePos)) {
            int side{rec_vec[i].pattern_side()};
            float initial_energy = rec_vec[i].get_energy();
            std::vector<int> initial_pattern = rec_vec[i].get_pattern_ref();

            for (int k = 0; k < 256; ++k) {
              float start_energy = rec_vec[i].get_energy();

              std::random_device r;
              std::default_random_engine eng{r()};
              std::uniform_int_distribution<int> dist{
                  0, side * side - 1};  // rivedi bene qui
              int n{dist(eng)};
              rec_vec[i].update(n);
              float end_energy = rec_vec[i].get_energy();

              if (end_energy > start_energy) {
                std::cerr << "Error: energy should not increase" << '\n';
              }
            }
            float final_energy = rec.get_energy();
            std::vector<int> final_pattern = rec.get_pattern_ref();

            sf::Image img = image_from_vector(final_pattern);
            texturerecall.loadFromImage(img);
            spriterecall.setTexture(texturerecall);

            bool emanuele = true;
          }
        }
      }
    }
    // fase di disegno per ogni frame
    window.clear(sf::Color::Red);
    // altro in  grafica prova

    // Disegna immagini originali
    for (auto& sprite : sprites) {
      window.draw(sprite);
    }

    // Disegna immagine distorta se presente
    if (is_noised) {
      window.draw(spritenoised);
    }

    if (showNoisedImage) {
      window.draw(spritenoised);
    }

    if (runningrecall) {
      window.draw(spriterecall);
    }

    if (emanuele) {
      window.draw(spriterecall);
    }

    window.display();
  }
  return 0;
}

}  // namespace hp
