#include "image_processor.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include "functions.hpp"

// costruttore di image_processor
ImageProcessor::ImageProcessor(std::string source,
                                         std::string destination) {
  this->sourceFolder = source; //salva le cartelle source e destination
  this->destinationFolder = destination;//prendendo le immagini da elaborare da source folder e salvando i risultati in destination folder
}

//funzione che elabora tutte le immagini che sono nella cartella di origine
void ImageProcessor::process() { //metodo process
  if (!std::filesystem::exists(this->destinationFolder)) {
    std::filesystem::create_directories(this->destinationFolder);
  }
//se la cartella destinazione non esiste la crea!
//itero su tutti i file png della cartella sorgente

  for (const auto& entry : std::filesystem::directory_iterator(this->sourceFolder)) {
    /**
     * Verifico che sia un file
     * e che sia png
     */
    if (entry.is_regular_file()) {
      auto path = entry.path(); //path=percorso
      std::string ext = path.extension().string();
      if (ext == ".png") {
        sf::Image img;
        // carico immagine
        img.loadFromFile(path.string());

        sf::Image elaborated = transform(img);//chiama la funzione transform()

        // salvo risultato in destinationFolder con lo stesso nome
        std::string outPath = //outpath=percorso per l'output
            this->destinationFolder + "/" + path.filename().string();
        elaborated.saveToFile(outPath);
      }
    }
  }
}

//classe derivata
ImmagineResized::ImmagineResized(std::string source, std::string destination)
    : ImageProcessor(source, destination) {}
//inizializza la sottoclasse passando i percorsi della classe base
sf::Image ImmagineResized::transform(const sf::Image& input) { 

    std::vector<sf::Color> colors = vector_from_image(input);  // vettore di sf::Color(immagine convertita in un vettore)
    std::vector<int> vector1 = blacknwhite(colors); //converte i colori in bianco e nero 
    
    int width = input.getSize().x;
    int height = input.getSize().y;      // -1 / 1
    std::vector<int> v_interpolated = bilinear_interpolation(vector1, width, height);
    //applica l'interpolazione bilineare x fare resize e renderla quadrata
    sf::Image image = image_from_vector(v_interpolated); //risorna l'immagine quadrata
    return image;
}


ImmagineZoomed::ImmagineZoomed(std::string source, std::string destination)
    : ImageProcessor(source, destination) {}

sf::Image ImmagineZoomed::transform(const sf::Image& input) {  
    std::vector<sf::Color> colori = vector_from_image(input);  // vettore di sf::Color
    std::vector<int> vector1 = blacknwhite(colori);            // -1 / 1
    std::vector<int> vector2 = zoom(vector1, 4);            // -1 / 1
    sf::Image image= image_from_vector(vector2);
    return image;
}



int main() {
    ImmagineResized interp("images/source", "images/resized");
    interp.process();
    ImmagineZoomed zoomma("images/resized", "images/zoomed");
    zoomma.process();
 }