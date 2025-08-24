#include "hebb.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "functions.hpp"

namespace hp {
Hebb::Hebb(std::string source, std::string destination) {
  this->sourceFolder = source;
  this->destinationFolder = destination;
}

void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {
  std::filesystem::path outdir(destinationFolder);

  // crea la cartella di destinazione se manca
  if (!std::filesystem::exists(outdir)) {
    if (!std::filesystem::create_directories(outdir)) {
      throw std::runtime_error{"[Hebb::save_matrix] cannot create directory ‘" +
                               outdir.string() + "'"};
    }
  }

  if (matrix.empty() || matrix[0].empty()) {
    throw std::runtime_error{"[Hebb::save_matrix] empty matrix"};
  }

  // percorso file completo
  std::filesystem::path outpath = outdir / "weight_matrix.txt";

  std::ofstream out(outpath, std::ios::out | std::ios::trunc);
  if (!out.is_open()) {
    throw std::runtime_error{
        "[Hebb::save_matrix] cannot open file for writing: " +
        outpath.string()};
  }

  out << std::fixed << std::setprecision(7);

  // scrivo ogni riga, valori separati da spazio
  for (const auto& row : matrix) {
    if (row.empty()) {
      throw std::runtime_error{
          "[Hebb::save_matrix] found an empty row in the matrix!"};
    }
    // scrive i valori delle righe separati da spazi
    for (size_t j = 0; j < row.size(); ++j) {
      if (j) out << ' ';
      out << row[j];
    }
    out << '\n';
  }
  out.close();

  // Stampa un messaggio di successo con le dimensioni della matrice e il
  // percorso del file.
  std::cout << "[Hebb::save_matrix] saved matrix to: " << outpath << "("
            << matrix.size() << " x " << matrix[0].size() << ")\n";
}

void Hebb::process() {
  std::vector<std::vector<int>> patterns;

  if (!std::filesystem::exists(sourceFolder)) {
    throw std::runtime_error{
        "[Hebb::process] ERROR: sourceFolder does not exist: " + sourceFolder +
        "\n"};
  } else {
    for (const auto& entry : std::filesystem::directory_iterator(
             sourceFolder)) {  // scorre tutti i file
      if (!entry.is_regular_file()) continue;
      auto path = entry.path();
      // accetta solo .png
      auto ext = path.extension().string();
      if (path.extension() != ".png") {
        throw std::runtime_error{"found non-png file: " +
                                 path.filename().string()};
      }
      sf::Image img;
      if (!img.loadFromFile(path.string())) {
        throw std::runtime_error{"[Hebb::process] failed to load image: " +
                                 path.string()};
      }
      auto colors = vector_from_image(img);
      auto pattern = blacknwhite(colors);
      patterns.push_back(std::move(pattern));
    }
  }

  std::cout << "[Hebb::process] total patterns collected: " << patterns.size()
            << "\n";

  if (patterns.empty()) {
    throw std::runtime_error{
        "[Hebb::process] no patterns found -> aborting process"};
  }
  std::vector<std::vector<float>> W;
  W = hebb(patterns);

  if (W.empty()) {
    throw std::runtime_error{
        "[Hebb::process] Weight matrix is empty -> aborting"};
  }
  save_matrix(W);
}
}  // namespace hp