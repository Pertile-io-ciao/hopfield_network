#include "recall.hpp"

#include <filesystem>
#include <fstream>
#include <random>
#include <cmath>

#include "functions.hpp"

Recall::Recall(const std::string& matrix_path) {
  this->matrix_Folder = matrix_path; //data
  this->weight_matrix = load_matrix();
}

std::vector<std::vector<float>> Recall::load_matrix() {
  std::filesystem::path intpath =
      this->matrix_Folder + "/" + "weight_matrix.txt";

  std::ifstream in(intpath, std::ios::in);
  //errore se non  apre
  if (!in) { 
    throw std::runtime_error(
        "Error: impossible to open file data/weight_matrix.txt");  // chat gpt
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

//da immagine a pattern
void Recall::initialize_from_image(const std::string& image_file) {
  this->image_clicked = image_file;
  std::filesystem::directory_entry entry(this->image_clicked);

  //guardo se l'immagine è png
  if (entry.is_regular_file()) {
    auto path = entry.path();  // path=percorso
    std::string ext = path.extension().string();
    if (ext == ".png") {
      sf::Image img;
      if (!img.loadFromFile(path.string())) {
        std::cerr << "Failed to load image: " << path << "\n";
      }
      
      auto colors = vector_from_image(img);
      std::vector<int> pattern = blacknwhite(colors);

      this->current_pattern = pattern;
    }
  }
}

//update considerando il neurone i che verrà scelto con random in graphics
void Recall::update(int i) {
  this->current_pattern =
      neuron_update(i, this->current_pattern, this->weight_matrix);
  this->energy = energy_function(this->current_pattern, this->weight_matrix);
}

float Recall::get_energy() const { return this->energy; }

//l=64
int Recall::pattern_side() const {
  int size{this->current_pattern.size()};
  return sqrt(size);
}

//ritorna pattern aggiornato a ogni richiesta
const std::vector<int>& Recall::get_pattern_ref() const {
  std::vector<int> pattern = this->current_pattern;
  return pattern;
}