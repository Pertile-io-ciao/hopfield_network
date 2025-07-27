#include <SFML/Graphics.hpp>
#include "graphics.hpp"

// Funzione per disegnare le texture nella finestra
int disegna(sf::Texture& texture1, sf::Texture& texture2, sf::Texture& texture3, sf::Texture& texture4, sf::Texture& distorta) {
    // Crea una finestra di rendering
    sf::RenderWindow window(sf::VideoMode(1900, 800), "Rete neurale di Hopfield");

    // Crea gli sprite associati alle texture
    sf::Sprite sprite1(texture1);
    sf::Sprite sprite2(texture2);
    sf::Sprite sprite3(texture3);
    sf::Sprite sprite4(texture4);
    sf::Sprite sprited(distorta);

    // Imposta le posizioni degli sprite
    sprite1.setPosition(0.f, 0.f);
    sprite2.setPosition(500.f, 0.f);
    sprite3.setPosition(1000.f, 0.f);
    sprite4.setPosition(1500.f, 0.f);
    sprited.setPosition(600.f, 400.f); // Posizione per l'immagine distorta

    // Loop principale della finestra
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Chiudi la finestra se viene richiesto
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Pulisce la finestra con il colore nero
        window.clear(sf::Color::Black);

        // Disegna tutti gli sprite
        window.draw(sprite1);
        window.draw(sprite2);
        window.draw(sprite3);
        window.draw(sprite4);
        window.draw(sprited); // Assicurati di disegnare anche l'immagine distorta

        // Visualizza il contenuto disegnato
        window.display();
    }

    return 0;
}