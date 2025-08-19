#include "recall.hpp"

#include <filesystem>
#include <fstream>
#include <random>

#include "functions.hpp"

Recall::Recall(std::string image, std::string matrix) {
  this->image_Folder = image;  // salva le cartelle sorgente
  this->matrix_Folder = matrix;
}

void Recall::process() {
  std::vector<std::vector<float>> weight_matrix = load_matrix();

  std::filesystem::directory_entry entry(this->image_Folder);
  /**
   * Verifico che sia un file
   * e che sia png
   */
  if (entry.is_regular_file()) {
    auto path = entry.path();  // path=percorso
    std::string ext = path.extension().string();
    if (ext == ".png") {
      sf::Image img;
      if (!img.loadFromFile(path.string())) {
        std::cerr << "Failed to load image: " << path << "\n";
      }
      // carico immagine
      else
        img.loadFromFile(path.string());

      auto colors = vector_from_image(img);
      std::vector<int> pattern = blacknwhite(colors);

      float energy = energy_function(pattern, weight_matrix);

      Status status{pattern, energy};

      for (int k = 0; k < 256; ++k) {
        // dinamica di aggiornamento neurone ed energia
        int n{pattern.size()};
        std::random_device r;
        std::default_random_engine eng{r()};
        std::uniform_real_distribution<int> dist{0, n * n};
        int i{dist(eng)};

        Status status = update_status(i, status, weight_matrix);

      }

      const std::vector<int>& final_pattern = status.pattern;
    int n = static_cast<int>(final_pattern.size());
    int side = static_cast<int>(std::round(std::sqrt(n)));
    if (side * side != n) {
        std::cerr << "Warning: pattern size is not a perfect square (" << n << ")\n";
    }

    sf::Image out_img;
    out_img.create(side, side, sf::Color::Black);

    for (int y = 0; y < side; ++y) {
        for (int x = 0; x < side; ++x) {
            int idx = y * side + x;
            int val = final_pattern[idx];
            // assumo pattern in {1, -1} oppure {1,0}
            if (val == 1) out_img.setPixel(x, y, sf::Color::White);
            else out_img.setPixel(x, y, sf::Color::Black);
        }
    }

    // crea directory di output se necessario
    std::filesystem::path out_p(this->image_Folder);
    if (out_p.has_parent_path() && !std::filesystem::exists(out_p.parent_path())) {
        std::filesystem::create_directories(out_p.parent_path());
    }

    if (!out_img.saveToFile(out_p.string())) {
        std::cerr << "Failed to save recalled image to: " << out_p << "\n";
    } else {
        std::cout << "Recalled image saved to: " << out_p << "\n";
    }
    
    }
  }
}

std::vector<std::vector<float>> Recall::load_matrix() {
  std::filesystem::path intpath =
      this->matrix_Folder + "/" + "weight_matrix.txt";

  std::ifstream in(intpath, std::ios::in);
  if (!in) {
    throw std::runtime_error(
        "Error: impossible to open file data/weight_matrix.txt");  // chat gpt
  }
  std::vector<std::vector<float>> W;  // Matrice da riempire
  std::string line;

  while (std::getline(in, line)) {  // Leggo una riga alla volta
    std::istringstream iss(line);   // Creo uno stream dalla stringa
    std::vector<float> row;
    int value;

    while (iss >> value) {  // Estraggo gli interi
      row.push_back(value);
    }

    if (!row.empty()) {
      W.push_back(row);  // Aggiungo la riga alla matrice
    }
  }

  return W;
}

Status Recall::update_status(int i, const Status& old_status, const std::vector<std::vector<float>>& W) {
  //std::vector<int> old_pattern = old_status.pattern;
  //float old_energy = old_status.energy;

  Status new_status;

  std::vector<int> new_pattern = new_status.pattern;
  float new_energy = new_status.energy;

  std::vector<int> new_pattern = neuron_update(i, old_status.pattern, W);
  float new_energy = energy_function(new_pattern, W);

  /// ERRORE SE L'ENERGIA AUMENTA

  return new_status;
}
