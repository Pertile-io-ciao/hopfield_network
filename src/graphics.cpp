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
//inizializzo 
bool showPopup = false;
bool showNoisedImage = false;
int selectedImageIndex = -1;
//pop up
sf::RectangleShape popupBox(sf::Vector2f(300.f, 250.f));
popupBox.setFillColor(sf::Color(64, 224, 208, 255)); 
popupBox.setPosition(800.f, 300.f); 

//opzioni del pop up
std::vector<sf::Text> popupOptions(4);
std::vector<std::string> optionLabels = {"Noised", "Vertical Cut", "Orizontal Cut", "Reverse"};

for (int i = 0; i < 4; ++i) {
    popupOptions[i].setFont(font);
    popupOptions[i].setString(optionLabels[i]);
    popupOptions[i].setCharacterSize(24);
    popupOptions[i].setFillColor(sf::Color::Black);
    popupOptions[i].setPosition(popupBox.getPosition().x + 30.f, popupBox.getPosition().y + 30.f + i * 50.f);
}

//inizializzo load image (pulsante alto a destra)
sf::RectangleShape load_images(sf::Vector2f(300.f, 40.f));
load_images.setFillColor(sf::Color(64, 224, 208, 255)); 
load_images.setPosition(1600.f, 0.f); 


sf::Text load_imagestxt;
 load_imagestxt.setFont(font);
 load_imagestxt.setString("Load images from your pc");
 load_imagestxt.setCharacterSize(15);
 load_imagestxt.setFillColor(sf::Color::Black);
 
sf::FloatRect textBounds = load_imagestxt.getLocalBounds();
load_imagestxt.setOrigin(textBounds.left + textBounds.width / 2.f,
                textBounds.top + textBounds.height / 2.f);

sf::Vector2f boxCenter = load_images.getPosition() + load_images.getSize() / 2.f;
load_imagestxt.setPosition(boxCenter);

  // Percorsi delle immagini
  std::vector<std::string> file_names = {"gigi.png", "kusozu.png", "noface.png", "totoro.png"};
  std::string zoomed = "resources/images/zoomed/";
  std::string zoomed_w_noise = "resources/images/zoomed_w_noise/";
//inizializzo i 4 sprites che andranno in alto (quelli normali)
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
    sprites[i].setPosition(i * 475.f+109.5f, 80.f);  // distanza tra immagini, le ho centrate
    noisedpath[i] = zoomed_w_noise + file_names[i];     //<-- prende l'immagine noised e zoomata e la salva
  }
  //inizializzo lo strite noised
  sf::Texture texturenoised;
  sf::Sprite spritenoised;
  bool is_noised = false;


  //ciclo principale che racchiude tutta la grafica che si vede a schermo
  while (window.isOpen()) {
    sf::Event event;      //carica tutti gli eventi utente (mouse, tastiera...)
      while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
        window.close();         //chiude la finestra se premo x

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {    //premere pulsante sinistro
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));    //mappa i miei spostamenti
        
        if (showPopup) { //inizializzato a false (sopra)
            if (showPopup) {
        for (int i = 0; i < 4; ++i) {   //ciclo per le opzioni
            sf::FloatRect bounds = popupOptions[i].getGlobalBounds(); //.getGlobalBounds() restituisce un sf::FloatRect, cioè un rettangolo che rappresenta la posizione e le dimensioni dell'oggetto
            if (bounds.contains(mousePos)) { //se premi l'opzione (i confini son definiti nella riga sopra)
                if (i == 0 && selectedImageIndex != -1) {  // se premo noised (1 opzione) e ho gia selezionato un immagine
                    if (!texturenoised.loadFromFile(noisedpath[selectedImageIndex])) {
                        std::cerr << "Errore nel caricamento dell'immagine corrotta: "
                                  << noisedpath[selectedImageIndex] << "\n";
                    } else {
                        spritenoised.setTexture(texturenoised);
                        float x = 283.f; //sto considerando che ci siano 3 immagini centrate ( noised; aggiornamento; finale)
                        float y = 450.f;
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

    window.draw(load_images);
    window.draw(load_imagestxt);

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