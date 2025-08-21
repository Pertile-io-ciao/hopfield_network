#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// funzione per verificare se un punto è dentro uno sprite
bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos) {
  return sprite.getGlobalBounds().contains(mousePos);
}
// sf::Sprite& sprite riferimento costante a uno sprite
// sf::Vector2f mousePos posizione del mouse in coordinate float
// sprite.getGlobalBounds() restituisce un sf::FloatRect con posizione e
// dimensioni dello sprite .contains(mousePos) verifica se il punto (mouse) è
// dentro il rettangolo

std::vector<bool> optionSelected(4, false);

int draw() {
  const float virtualWidth = 1900.f;
  const float virtualHeight = 800.f;

  sf::RenderWindow window(sf::VideoMode(virtualWidth, virtualHeight),
                          "hopfield_network", sf::Style::Close);
  // sf::RenderWindow è classe di sfml x creare una finestra grafica
  // sf::VideoMode definisce la grandezza (la risoluzione della finestra)
  // sf::Style::Close è il tipo della finestra (puo essere solo chiusa, nn
  // ridimensionata/ingrandita)

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

  // inizializzo, sono le variabili di stato
  // bool showPopup = false;        // mostra/nasconde un pop-up
  bool showNoisedImage = false;  // mostra l'immagine corrotta
  // int selectedImageIndex = -1;   // indica quale immagine è stata cliccata

  // Percorsi delle immagini
  std::vector<std::string> file_names = {"gigi.png", "kusozu.png", "noface.png",
                                         "totoro.png"};
  std::string zoomed = "resources/images/zoomed/";
  std::string zoomed_w_noise = "resources/images/zoomed_w_noise/";

  // inizializzo i 4 sprites che andranno in alto (quelli normali)
  std::vector<sf::Texture> textures(4);  // carica immagini
  std::vector<sf::Sprite> sprites(4);    // oggetti sulla finestra grafica
  std::vector<std::string> noisedpath(
      4);  // percorsi delle versioni corrotte delle immagini

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
          }
        }
      }
    }

    window.clear(sf::Color::Black);

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