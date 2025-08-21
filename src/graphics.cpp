#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

namespace pf{
// funzione per verificare se un punto è dentro uno sprite
bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos) {
  return sprite.getGlobalBounds().contains(mousePos);
}

//std::vector<bool> optionSelected(4, false);

int draw() {
  const float virtualWidth = 1900.f;
  const float virtualHeight = 800.f;

  sf::RenderWindow window(sf::VideoMode(virtualWidth, virtualHeight),
                          "hopfield_network", sf::Style::Close);
  
  sf::View view(sf::FloatRect(0, 0, virtualWidth, virtualHeight));
  window.setView(view);
  // sf::View è la vista virtuale
  // sf::FloatRect area visibile della vista
  // window.setView(view) assegna la vista alla finestra
  // il font serve nei pop up

  sf::Font font;
  if (!font.loadFromFile("resources/arial.ttf")) {
    std::cerr << "Error in font loading\n";
  }

  
  bool showNoisedImage = false;  

  //Percorsi delle immagini
  std::vector<std::string> file_names = {"gigi.png", "kusozu.png", "noface.png",
                                         "totoro.png"};
  std::string zoomed = "resources/images/zoomed/";
  std::string zoomed_w_noise = "resources/images/zoomed_w_noise/";

  // inizializzo i 4 sprites che andranno in alto (quelli normali)
  std::vector<sf::Texture> textures(4);  
  std::vector<sf::Sprite> sprites(4);    
  std::vector<std::string> noisedpath(4);  // percorsi delle versioni corrotte delle immagini

  // carica immagini originali
  for (int i = 0; i < 4; ++i) {
    if (!textures[i].loadFromFile(zoomed + file_names[i])) {
      std::cerr << "Errore nel caricamento di: "
                << zoomed_w_noise + file_names[i] << "\n";
      return -1;
    }
    sprites[i].setTexture(textures[i]);
    sprites[i].setPosition(i * 475.f + 109.5f,
                           80.f);  // distanza tra immagini, le ho centrate
    noisedpath[i] =
        zoomed_w_noise +
        file_names[i];  //<-- prende l'immagine noised e zoomata e la salva
  }

  // inizializzo lo sprite noised
  sf::Texture texturenoised;
  sf::Sprite spritenoised;
  bool is_noised = false;

  /*
  // inizializo l'oggetto recall
  Recall rec("data");

  // inizializzo lo sprite in evoluzione
  sf::Texture texturerecall;
  sf::Sprite spriterecall;
  bool runningrecall = false;

  // inizializzo la scritta per la funzione energia
  sf::Text energyText;
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
            if (!texturenoised.loadFromFile(noisedpath[i])) {
              std::cerr << "Errore nel caricamento dell'immagine corrotta: "
                        << noisedpath[i] << "\n";
            } else {
              spritenoised.setTexture(texturenoised);
              is_noised = true;
            }

            // Mostra immagine (modificata o originale)
            spritenoised.setTexture(texturenoised);
            spritenoised.setPosition(283.f, 450.f);
            showNoisedImage = true;
            // showPopup = false;
            /* //l'ho spostato di una { più in alto
          if (isSpriteClicked(spritenoised, mousePos)) {
            rec.initialize_from_image(
                noisedpath[i]);  // inizializza il pattern corrotto
            int side{rec.pattern_side()};

            texturerecall.create(side, side);
            spriterecall.setTexture(texturerecall);
            spriterecall.setPosition(283.f, 800.f);
            runningrecall = true;
          }
            float initial_energy = rec.get_energy();
          std::vector<int> initial_pattern = rec.get_pattern_ref();

          float final_energy = 0.0;
          std::vector<int> final_pattern(side * side, 0);

          while (initial_pattern != final_pattern) {
            for (int k = 0; k < 256; ++k) {
              float start_energy = rec.get_energy();

              int n{rec.pattern_side()};
              std::random_device r;
              std::default_random_engine eng{r()};
              std::uniform_int_distribution<int> dist{
                  0, n * n - 1};  // rivedi bene qui
              int i{dist(eng)};
              rec.update(i);
              float end_energy = rec.get_energy();

              if (end_energy > start_energy) {
                std::cerr << "Error: energy should not increase" << '\n';
              }
            }
            float final_energy = rec.get_energy();
            std::vector<int> final_pattern = rec.get_pattern_ref();

            sf::Image img = image_from_vector(final_pattern);
            texturerecall.loadFromImage(img);

            energyText.setString("Energy: " +
                                 std::to_string(rec.get_energy()));  // chat*/
          }
        }
      }
    }

    window.clear(sf::Color::Black);
    //altro in  grafica prova
    
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

    window.display();
  }

  return 0;
}

}