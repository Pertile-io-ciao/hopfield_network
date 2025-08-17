#include "hebb.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "functions.hpp"

Hebb::Hebb(std::string source) {
  this->sourceFolder = source;  // salva le cartelle sorgente
}

void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) {
  std::ofstream out("data/weight_matrix.txt");
  if (!out) {
        throw std::runtime_error("Error: impossible to open file data/weight_matrix.txt");  //chat gpt
    }
  for (const auto& row : matrix) {
    for (float value : row) {
      out << value << " ";
    }
    out << "\n";
  }
  out.close();
}

// funzione che elabora tutte le immagini che sono nella cartella di origine
void Hebb::creatematrix() {  // metodo process

  std::vector<std::vector<int>>
      vector_of_vectors;  //{4, std::vector<int>(64*64, 0)};
  vector_of_vectors.reserve(4);

  for (const auto& entry :
       std::filesystem::directory_iterator(this->sourceFolder)) {
    /**
     * Verifico che sia un file
     * e che sia png
     */
    if (entry.is_regular_file()) {
      auto path = entry.path();  // path=percorso
      std::string ext = path.extension().string();
      if (ext == ".png") {
        sf::Image img;
        // carico immagine
        if (!img.loadFromFile(path.string())) {
          std::cerr << "Errore caricamento immagine: " << path << "\n";
          continue;
        }

        // Trasformazione dell'immagine
        /*
        sf::Image elaborated = img;
        std::vector<sf::Color> colors = vector_from_image(elaborated);
        std::vector<int> vector1 = blacknwhite(colors);
        int width = elaborated.getSize().x;
        int height = elaborated.getSize().y;
        std::vector<int> v_interpolated =
            bilinear_interpolation(vector1, width, height);
        std::vector<std::vector<int>> patterns =
            vector_of_vectors(v_interpolated);
        std::vector<std::vector<float>> weight_matrix = hebb(patterns);
        save_matrix(weight_matrix);

        */

        sf::Image elaborated = img;
        std::vector<sf::Color> colors = vector_from_image(elaborated);
        std::vector<int> pattern = blacknwhite(colors);
        vector_of_vectors.push_back(pattern);
      }
    }
  }
  std::vector<std::vector<float>> weight_matrix = hebb(vector_of_vectors);
  save_matrix(weight_matrix);
}
