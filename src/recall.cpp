#include "recall.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "functions.hpp"

namespace hp {
Recall::Recall(const std::string& matrix_path) {
  this->matrix_Folder = matrix_path;  // data
  this->weight_matrix = load_matrix();
}

std::vector<std::vector<float>> Recall::load_matrix() {
  std::filesystem::path intpath =
      this->matrix_Folder + "/" + "weight_matrix.txt";
  std::ifstream in(intpath, std::ios::in);
  if (!in) {
    throw std::runtime_error{
        "[Recall::load_matrix] error: impossible to open file " +
        intpath.string()};
  }
  std::vector<std::vector<float>> W;  // matrice da riempire
  std::string line;

  while (std::getline(in, line)) {  // leggo una riga alla volta
    std::istringstream iss(line);
    std::vector<float> row;
    float value;

    while (iss >> value) {  // estraggo gli interi
      row.push_back(value);
    }

    if (!row.empty()) {
      W.push_back(row);
    }
  }

  return W;
}

// da immagine a pattern
void Recall::initialize_from_image(const std::string& image_file) {
  this->image_clicked = image_file;
  sf::Image img;
  if (!img.loadFromFile(image_file)) {
    throw std::runtime_error{
        "[Recall::initialize_from_image] failed to load image: " + image_file};
  }
  std::vector<sf::Color> colors = vector_from_image(img);
  std::vector<int> pattern = blacknwhite(colors);
  this->current_pattern = pattern;
  std::cout << "image file: " << image_file << '\n';
  std::cout << "image size: " << img.getSize().x << "x" << img.getSize().y
            << '\n';
  std::cout << "pattern size: " << pattern.size() << '\n';
  if (pattern.size() != weight_matrix.size()) {
    throw std::runtime_error{
        "[Recall::initialize_from_image] pattern size and weight matrix size "
        "mismatch!"};
  }
  this->energy = energy_function(this->current_pattern, this->weight_matrix);
}

// update considerando il neurone i che verrà aggiornato
void Recall::update(int neuron_index) {
  current_pattern = neuron_update(neuron_index, current_pattern, weight_matrix);
}

void Recall::compute_energy() {
  energy = energy_function(current_pattern, weight_matrix);
}

int Recall::pattern_side() const {
  auto size = this->current_pattern.size();
  return static_cast<int>(std::sqrt(static_cast<double>(size)));
}

const std::vector<int>& Recall::get_pattern_ref() const {
  return this->current_pattern;
}

float Recall::get_energy() const { return this->energy; }
}  // namespace hp
