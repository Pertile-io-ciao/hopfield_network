#include <SFML/Graphics.hpp> 

#include <iostream>
#include <vector>

// Funzione per verificare se un punto è dentro uno sprite
bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos) {
  return sprite.getGlobalBounds().contains(mousePos);
}



int draw() {
  sf::RenderWindow window(sf::VideoMode(1900, 800), "hopfield_network");
  
//il font serve nei pop up
sf::Font font;
if (!font.loadFromFile("resources/arial.ttf")) {
    std::cerr << "Errore nel caricamento del font\n";
}

bool showPopup = false;
bool showNoisedImage = false;
int selectedImageIndex = -1;

sf::RectangleShape popupBox(sf::Vector2f(300.f, 200.f));
popupBox.setFillColor(sf::Color(64, 224, 208, 255)); 
popupBox.setPosition(800.f, 300.f); // centro approssimativo

std::vector<sf::Text> popupOptions(3);
std::vector<std::string> optionLabels = {"Noised", "Vertical Cut", "Orizontal Cut"};

for (int i = 0; i < 3; ++i) {
    popupOptions[i].setFont(font);
    popupOptions[i].setString(optionLabels[i]);
    popupOptions[i].setCharacterSize(24);
    popupOptions[i].setFillColor(sf::Color::Black);
    popupOptions[i].setPosition(popupBox.getPosition().x + 30.f, popupBox.getPosition().y + 30.f + i * 50.f);
}

  // Percorsi delle immagini
  std::vector<std::string> file_names = {"gigi.png", "kusozu.png", "noface.png",
                                         "totoro.png"};
  std::string zoomed = "resources/images/zoomed/";
  std::string zoomed_w_noise = "resources/images/zoomed_w_noise/";

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
    sprites[i].setPosition(i * 475.f+109.5f, 0.f);  // distanza tra immagini
    noisedpath[i] = zoomed_w_noise + file_names[i];
  }

  sf::Texture texturenoised;
  sf::Sprite spritenoised;
  bool is_noised = false;

  while (window.isOpen()) {
    sf::Event event;      //carica tutti gli eventi utente (mouse, tastiera...)
      while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
        window.close();         //chiude la finestra se premo x

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {    //premere pulsante sinistro
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));    //mappa i miei spostamenti
        
        if (showPopup) {
            if (showPopup) {
        for (int i = 0; i < 3; ++i) {
            sf::FloatRect bounds = popupOptions[i].getGlobalBounds();
            if (bounds.contains(mousePos)) {
                if (i == 0 && selectedImageIndex != -1) {  
                    // Se clicco la prima opzione e un'immagine è selezionata,
                    // carica la texture noised e mostra l'immagine
                    if (!texturenoised.loadFromFile(noisedpath[selectedImageIndex])) {
                        std::cerr << "Errore nel caricamento dell'immagine corrotta: "
                                  << noisedpath[selectedImageIndex] << "\n";
                    } else {
                        spritenoised.setTexture(texturenoised);
                        float x = (window.getSize().x - texturenoised.getSize().x) / 2.f;
                        float y = 500.f;
                        spritenoised.setPosition(x, y);
                        showNoisedImage = true;
                    }
                }

                showPopup = false;  // chiudi popup dopo il click
                break;
            }
        }
      }
    } else {
        // controllo clic sulle immagini
        for (int i = 0; i < 4; ++i) {
            if (isSpriteClicked(sprites[i], mousePos)) {
                selectedImageIndex = i;  // salva quale immagine hai cliccato
                showPopup = true;        // mostra il popup
                showNoisedImage = false; // nascondi immagine noised per ora
                break;
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
   
       if (showPopup) {
    window.draw(popupBox);
    for (auto& text : popupOptions) {
        window.draw(text);
    }
}
if (showNoisedImage) {
    window.draw(spritenoised);
}

    window.display();
}

}
  return 0;
}