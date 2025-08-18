#include "hebb.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "functions.hpp"

Hebb::Hebb(std::string source, std::string destination) {
  this->sourceFolder = source;  // salva le cartelle sorgente
  this->destinationFolder = destination;
}

/*
void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {
  std::ofstream out("");
  if (!out) {
    throw std::runtime_error(
        "Error: impossible to open file data/weight_matrix.txt");  // chat gpt
  }
  for (const auto& row : matrix) {
    for (float value : row) {
      out << value << " ";
    }
    out << "\n";
  }
  out.close();
}
*/

void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {
  std::filesystem::path outpath(this->destinationFolder);
  if (!std::filesystem::exists(outpath)) {
    std::filesystem::create_directories(outpath);
  }

  std::ofstream out(outpath);
  if (!out) {
    throw std::runtime_error(
        "Error: impossible to open file data/weight_matrix.txt");  // chat gpt
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
void Hebb::process() {  // metodo process

  std::vector<std::vector<int>> patterns;

  if (!std::filesystem::exists(this->destinationFolder)) {
    std::filesystem::create_directories(this->destinationFolder);
  }

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
          std::cerr << "Failed to load image: " << path << "\n";
          continue;
        }
        else img.loadFromFile(path.string());

        auto colors = vector_from_image(img);
        auto pattern = blacknwhite(colors);
        patterns.push_back(pattern);
      }
    }
  }
  auto weight_matrix = hebb(patterns);
  save_matrix(weight_matrix);
}
