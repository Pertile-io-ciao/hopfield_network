#include <iostream>

#include "functions.hpp"
#include "recall.hpp"

namespace hp {

// funzione per verificare se un punto è dentro uno sprite
bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos) {
  return sprite.getGlobalBounds().contains(mousePos);
}

int draw() {
  Recall rec("data");
  int selected_image_index = -1;
  int start_index = 0;
  const float virtualWidth = 1900.f;
  const float virtualHeight = 800.f;

  sf::RenderWindow window(sf::VideoMode(virtualWidth, virtualHeight),
                          "hopfield_network", sf::Style::Close);

  sf::View view(sf::FloatRect(0, 0, virtualWidth, virtualHeight));
  window.setView(view);

  sf::Font font;
  if (!font.loadFromFile("resources/arial.ttf")) {
    throw std::runtime_error(
        "[draw] error: impossible loading font from resources/arial.ttf");
  }

  bool showNoisedImage = false;

  // Percorsi delle immagini
  std::vector<std::string> file_names = {"battilana.png", "battilana2.png",
                                         "ferrari.png", "lanzi.png"};

  std::string zoomed = "resources/images/zoomed/";
  std::string zoomed_w_noise = "resources/images/zoomed_w_noise/";
  std::string noised = "resources/images/noised/";
  std::string resized = "resources/images/resized/";

  // inizializzo i 4 sprites che andranno in alto (quelli normali)
  std::vector<sf::Texture> textures(4);
  std::vector<sf::Sprite> sprites(4);
  std::vector<std::string> zoomed_w_noisepath(
      4);  // percorsi delle versioni corrotte delle immagini

  // percorsi per le immaagini da cui ricavo i pattern originali
  std::vector<std::vector<int>> original_patterns(4);
  std::vector<sf::Image> images(4);
  std::vector<sf::Texture> textures_original(4);
  std::vector<std::vector<sf::Color>> colors_original(4);

  // carica immagini originali
  for (int i = 0; i < 4; ++i) {
    if (!textures[i].loadFromFile(zoomed + file_names[i])) {
      throw std::runtime_error("[draw] error: impossible loading image from " +
                               zoomed + file_names[i]);
    }
    sprites[i].setTexture(textures[i]);
    sprites[i].setPosition(i * 475.f + 109.5f,
                           80.f);  // distanza tra immagini
    zoomed_w_noisepath[i] =
        zoomed_w_noise +
        file_names[i];  

    if (!textures_original[i].loadFromFile(resized + file_names[i])) {
      throw std::runtime_error("[draw] error: impossible loading image from " +
                               resized + file_names[i]);
    }

    images[i] = textures_original[i].copyToImage();
    colors_original[i] = vector_from_image(images[i]);
    original_patterns[i] = blacknwhite(colors_original[i]);
  }

  // inizializzo lo sprite noised, in basso a sx
  sf::Texture texturenoised;
  sf::Sprite spritenoised;
  bool is_noised = false;

  // inizializzo lo sprite in evoluzione
  sf::Texture texturerecall;
  sf::Sprite spriterecall;
  int runningrecall = 0;

  std::random_device r;
  std::default_random_engine eng{r()};

  // inizializzo la scritta per la funzione energia
  sf::Text energyText;
  energyText.setFont(font);
  energyText.setCharacterSize(35);
  energyText.setFillColor(sf::Color::Black);
  energyText.setPosition(1280.f, 530.f);

  sf::RectangleShape background;
  background.setFillColor(sf::Color(255, 255, 255, 200));
  background.setSize(sf::Vector2f(energyText.getLocalBounds().width + 10,
                                  energyText.getLocalBounds().height + 10));
  background.setPosition(energyText.getPosition().x,
                         energyText.getPosition().y);

  // ciclo principale che racchiude tutta la grafica che si vede a schermo
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
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

      if (event.type == sf::Event::Closed) window.close();

      // gestione click
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (int i = 0; i < 4; ++i) {
          if (isSpriteClicked(sprites[i], mousePos)) {
            // Carica immagine corrotta corrispondente
            if (!texturenoised.loadFromFile(zoomed_w_noisepath[i])) {
              throw std::runtime_error(
                  "[draw] error: impossible loading image from " +
                  zoomed_w_noisepath[i]);
            } else {
              spritenoised.setTexture(texturenoised);
              spritenoised.setPosition(283.f, 450.f);
              showNoisedImage = true;
              is_noised = true;
              selected_image_index = i;  // immagine che sarà usata dalla rete
              runningrecall = 0;         // la rete non parte ancora
            }
          }
        }

        if (isSpriteClicked(spritenoised, mousePos)) {
          // carica immagine corrotta corrispondente
          rec.initialize_from_image(noised + file_names[selected_image_index]);
          sf::Image img = image_from_vector(zoom(rec.get_pattern_ref()));
          std::cout << "Image size: " << img.getSize().x << "x"
                    << img.getSize().y << '\n';
          sf::Vector2u imgSize = img.getSize();
          texturerecall.create(imgSize.x, imgSize.y);
          texturerecall.loadFromImage(img);
          spriterecall.setTexture(texturerecall);
          spriterecall.setPosition(815.f, 450.f);
          runningrecall = 1;  // ora la rete può partire al prossimo frame
          std::cout << "runningrecall set to true!" << '\n';
          start_index = 0;
        }
      }  

    }  

    // dinamica di aggiornamento
    if (runningrecall == 1) {
      for (int q = 0; q < 4; ++q) {
        int side = rec.pattern_side();
        int total_neurons = side * side;
        int neurons_per_frame = 100;
        std::vector<int> previous_pattern(total_neurons, -1);

        for (int k = 0; k < neurons_per_frame; ++k) {
          std::uniform_int_distribution<int> distrib{0, total_neurons - 1};
          int neuron_to_update = distrib(eng);

          rec.update(neuron_to_update);
        }

        start_index = (start_index + neurons_per_frame) % total_neurons;

        previous_pattern = rec.get_pattern_ref();

        sf::Image img = image_from_vector(zoom(rec.get_pattern_ref()));
        texturerecall.update(img);
        spriterecall.setTexture(texturerecall);

        for (int i = 0; i < 4; ++i) {
          if (previous_pattern == original_patterns[i]) {
            std::cout << "convergence!" << '\n';
            runningrecall = 2; // convergenza: fine della dinamica di aggiornamento
          }
        }
      }
      // aggiornamento energia
      rec.compute_energy();
      energyText.setString("Energy: " + std::to_string(rec.get_energy()));

      background.setSize(sf::Vector2f(energyText.getLocalBounds().width + 20,
                                      energyText.getLocalBounds().height + 20));
      background.setPosition(energyText.getPosition().x - 10,
                             energyText.getPosition().y - 5);
    }

    // fase di disegno per ogni frame

    window.clear(sf::Color(95, 158, 160));

    // Disegna immagini originali
    for (auto& sprite : sprites) {
      window.draw(sprite);
    }

    // Disegna immagine distorta se presente
    if (is_noised && showNoisedImage) {
      window.draw(spritenoised);
    }

    if (runningrecall == 1 || runningrecall == 2) {
      window.draw(spriterecall);
      window.draw(background);
      window.draw(energyText);
    }

    window.display();
  }  
  return 0;
}  

}  // namespace hp