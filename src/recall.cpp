#include "recall.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <random>

#include "functions.hpp"

Recall::Recall(const std::string& matrix_path) {
  this->matrix_Folder = matrix_path;  // data
  this->weight_matrix = load_matrix();
}

std::vector<std::vector<float>> Recall::load_matrix() {
  std::filesystem::path intpath =
      this->matrix_Folder + "/" + "weight_matrix.txt";

  std::ifstream in(intpath, std::ios::in);
  // errore se non  apre
  if (!in) {
    throw std::runtime_error("[Recall::load_matrix] error: impossible to open file " + intpath.string());
 //chat gpt
  }
  std::vector<std::vector<float>> W;  // Matrice da riempire
  std::string line;

  while (std::getline(in, line)) {  // Leggo una riga alla volta
    std::istringstream iss(line);   // Creo uno stream dalla stringa
    std::vector<float> row;
    float value;

    while (iss >> value) {  // Estraggo gli interi
      row.push_back(value);
    }

    if (!row.empty()) {
      W.push_back(row);  // Aggiungo la riga alla matrice
    }
  }

  return W;
}

// da immagine a pattern
void Recall::initialize_from_image(const std::string& image_file) {
  this->image_clicked = image_file;
  sf::Image img;
  if (!img.loadFromFile(image_file)) {
    throw std::runtime_error("[Recall::initialize_from_image] failed to load image: " + image_file);
  }
  std::vector<sf::Color> colors = vector_from_image(img);
  std::vector<int> pattern = blacknwhite(colors);
  this->current_pattern = pattern;
  std::cout << "Image file: " << image_file << '\n';
  std::cout << "Image size: " << img.getSize().x << "x" << img.getSize().y
            << '\n';
  std::cout << "Pattern size: " << pattern.size() << '\n';
  if (pattern.size() != weight_matrix.size()) {
    throw std::runtime_error("[Recall::initialize_from_image] pattern size and weight matrix size mismatch!"); 
  }
  // this->energy = energy_function(this->current_pattern, this->weight_matrix);
}

// update considerando il pattern i che verrà scelto cliccando sull'immagine in
// graphics
void Recall::update(int neuron_index) {
  current_pattern = neuron_update(neuron_index, current_pattern, weight_matrix);
  // energy = energy_function(current_pattern, weight_matrix);
}

// float Recall::get_energy() const { return this->energy; }

// l=56
int Recall::pattern_side() const {
  auto size = this->current_pattern.size();
  return static_cast<int>(std::sqrt(static_cast<double>(size)));
}

// ritorna pattern aggiornato a ogni richiesta
const std::vector<int>& Recall::get_pattern_ref() const {
  // std::vector<int> pattern = this->current_pattern;
  return this->current_pattern;
}
