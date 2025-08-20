#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Recall.hpp"

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

  // crazione pop up
  /*sf::RectangleShape popupBox(sf::Vector2f(300.f, 280.f));  // rettangolo
                                                            // grafico
  popupBox.setFillColor(
      sf::Color(64, 224, 208, 255));   // imposta il colore (RGBA)
  popupBox.setPosition(800.f, 300.f);  // posizione in coordinate virtuali

  // opzioni del pop up (testo)
  std::vector<sf::Text> popupOptions(4);
  std::vector<std::string> optionLabels = {"Noised", "Vertical Cut",
                                           "Orizontal Cut", "Reverse"};

  for (int i = 0; i < 4; ++i) {
    popupOptions[i].setFont(font);
    popupOptions[i].setString(optionLabels[i]);
    popupOptions[i].setCharacterSize(24);
    popupOptions[i].setFillColor(sf::Color::Black);
    popupOptions[i].setPosition(popupBox.getPosition().x + 30.f,
                                popupBox.getPosition().y + 30.f + i * 50.f);
  }
  // imposta stile, font, dimensione, colore e posizione a ciascuna voce del pop
  // up

  sf::Text applyButton;
  applyButton.setFont(font);
  applyButton.setString("Apply");
  applyButton.setCharacterSize(24);
  applyButton.setFillColor(sf::Color::White);
  applyButton.setPosition(popupBox.getPosition().x + 100.f,
                          popupBox.getPosition().y + 230.f);*/

  // inizializzo load image (pulsante in alto a destra)
  sf::RectangleShape load_images(sf::Vector2f(300.f, 40.f));
  load_images.setFillColor(sf::Color(64, 224, 208, 255));
  load_images.setPosition(1600.f, 0.f);

  // impostazioni del testo
  sf::Text load_imagestxt;  // crea un oggetto di tipo sf::Text (testo
                            // visualizzabile)
  load_imagestxt.setFont(font);
  load_imagestxt.setString(
      "Load images from your pc");  // imposta la striscia di testo da mostrare
  load_imagestxt.setCharacterSize(15);
  load_imagestxt.setFillColor(sf::Color::Black);

  sf::FloatRect textBounds =
      load_imagestxt
          .getLocalBounds();  // rettangolo (FloatRect) che rappresenta le
                              // dimensioni locali del testo
  load_imagestxt.setOrigin(textBounds.left + textBounds.width / 2.f,
                           textBounds.top + textBounds.height / 2.f);
  // imposta l'origine del testo x centrarlo rispetto al rettangolo

  sf::Vector2f boxCenter =
      load_images.getPosition() + load_images.getSize() / 2.f;
  load_imagestxt.setPosition(boxCenter);

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

  // inizializzo lo sprite in evoluzione
  sf::Texture texturerecall;
  sf::Sprite spriterecall;
  bool runningrecall = false;

  // inizializzo la scritta per la funzione energia
  sf::Text energyText;
  energyText.setFont(font);
  energyText.setCharacterSize(18);
  energyText.setFillColor(sf::Color::White);
  energyText.setPosition(500.f, 1400.f);

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

            /*if (showPopup) {  // inizializzato a false (sopra)
              if (showPopup) {
                for (int i = 0; i < 4; ++i) {  // ciclo per le opzioni
                  sf::FloatRect bounds =
                      popupOptions[i]
                          .getGlobalBounds();  //.getGlobalBounds() restituisce
              un
                                               // sf::FloatRect, cioè un
              rettangolo
                                               // che rappresenta la posizione e
              le
                                               // dimensioni dell'oggetto
                  if (bounds.contains(mousePos)) {
                    optionSelected[i] = !optionSelected[i];  // toggle: ON <->
              OFF
                  }
                }
                // Controllo clic su "Apply"
                if (applyButton.getGlobalBounds().contains(mousePos)) {
                  if (selectedImageIndex != -1) {
                    std::string imageToLoad = "";
                    bool applied = false;
                    // Ordine di priorità: Noised → Vertical → Orizontal →
              Reverse if (optionSelected[0]) { imageToLoad = zoomed_w_noise +
              file_names[selectedImageIndex]; applied = true;
                    }
                    else if (optionSelected[1]) {
                      imageToLoad = "resources/images/vertical_cut/" +
                                    file_names[selectedImageIndex];
                      applied = true;
                    }
                    else if (optionSelected[2]) {
                      imageToLoad = "resources/images/orizontal_cut/" +
                                    file_names[selectedImageIndex];
                      applied = true;
                    }
                    else if (optionSelected[3]) {
                      imageToLoad = "resources/images/reverse/" +
                                    file_names[selectedImageIndex];
                      applied = true;
                    }

                    // Se nessuna opzione selezionata, non fare nulla
                    if (!applied) {
                      std::cout << "Nessuna opzione selezionata.\n";
                      return 0;
                    }*/

            // Prova a caricare l'immagine modificata

            // Fallback: carica immagine originale
            /*if (!texturenoised.loadFromFile(zoomed +
                                            file_names[selectedImageIndex])) {
              std::cerr
                  << "Errore anche nel caricamento immagine originale\n";
              return -1;
            } else {
              std::cout << "Caricata immagine originale come fallback.\n";
            }
          }*/

            // Mostra immagine (modificata o originale)
            spritenoised.setTexture(texturenoised);
            spritenoised.setPosition(283.f, 450.f);
            showNoisedImage = true;
            // showPopup = false;
          }

          if (isSpriteClicked(spritenoised, mousePos)) {
            Recall rec("data/weight_matrix.txt");
            rec.initialize_from_image(noisedpath[i]);
            int side{64};

            texturerecall.create(side, side);
            spriterecall.setTexture(texturerecall, true);


          }
        }
      }
    }
    /*else {
      // controllo clic sulle immagini
      for (int i = 0; i < 4; ++i) {
        if (isSpriteClicked(sprites[i], mousePos)) {
          selectedImageIndex = i;   // salva quale immagine hai cliccato
          showPopup = true;         // mostra il popup
          showNoisedImage = false;  // nascondi immagine noised per ora
          break;
        }
      }
    }
  }*/

    window.clear(sf::Color::Black);

    // Disegna immagini originali
    for (auto& sprite : sprites) {
      window.draw(sprite);
    }

    window.draw(load_images);
    window.draw(load_imagestxt);

    // Disegna immagine distorta se presente
    if (is_noised) {
      window.draw(spritenoised);
    }

    /*if (showPopup) {
      window.draw(popupBox);
      /*for (auto& text : popupOptions) {
        window.draw(text);
      }
      for (int i = 0; i < 4; ++i) {
        popupOptions[i].setFillColor(optionSelected[i] ? sf::Color::Green
                                                       : sf::Color::Black);
        window.draw(popupOptions[i]);
      }

      window.draw(applyButton);
    }*/
    if (showNoisedImage) {
      window.draw(spritenoised);
    }

    window.display();

  }  // si chiude la finestra (termina while window is open)

  return 0;
}