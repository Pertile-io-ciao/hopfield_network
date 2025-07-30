#include <SFML/Graphics.hpp> 

#include <iostream>
#include <vector>

// Funzione per verificare se un punto è dentro uno sprite
bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos) {
  return sprite.getGlobalBounds().contains(mousePos);
}

int main() {
  sf::RenderWindow window(sf::VideoMode(1900, 800), "hopfield_network");
  

  // Percorsi delle immagini
  std::vector<std::string> file_names = {"gigi.png", "kusozu.png", "noface.png",
                                         "totoro.png"};
  std::string zoomed = "images/zoomed/";
  std::string zoomed_w_noise = "images/zoomed_w_noise/";

  std::vector<sf::Texture> textures(4);
  std::vector<sf::Sprite> sprites(4);
  std::vector<std::string> noisedpath(4);

  // Carica immagini originali
  for (int i = 0; i < 4; ++i) {
    if (!textures[i].loadFromFile(zoomed + file_names[i])) {
      std::cerr << "Errore nel caricamento di: "
                << zoomed_w_noise + file_names[i] << "\n";
      return -1;
    }
    sprites[i].setTexture(textures[i]);
    sprites[i].setPosition(i * 475.f, 0.f);  // distanza tra immagini
    noisedpath[i] = zoomed_w_noise + file_names[i];
  }

  sf::Texture texturenoised;
  sf::Sprite spritenoised;
  bool is_noised = false;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (int i = 0; i < 4; ++i) {
          if (isSpriteClicked(sprites[i], mousePos)) {
            // Carica immagine corrotta corrispondente
            if (!texturenoised.loadFromFile(noisedpath[i])) {
              std::cerr << "Errore nel caricamento dell'immagine corrotta: "
                        << noisedpath[i] << "\n";
            } else {
              spritenoised.setTexture(texturenoised);
              // Centra la versione corrotta nella parte bassa
              float x = (window.getSize().x - texturenoised.getSize().x) / 2.f;
              float y = 500.f;  // posizione verticale bassa
              spritenoised.setPosition(x, y);
              is_noised = true;
            }
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

    window.display();
  }

  return 0;
}
/*ho molte domande: il main dovrebbe stare nel main.cpp?
  su graphics.hpp non ho scritto nulla, che ci devo scrivere? (questo forse ce
  la posso fare) 
  ci sono dell cose inglese da cambiare 
  per compilare con cmake, mi dava una sprta di conflitto perche cerano due main-->ho tolto
  imageprocessor.cpp dagli eseguibili, pero cosi non mi convince di andare ogni volta a cambiarli... 
  domani commento tutto per bene!
  */