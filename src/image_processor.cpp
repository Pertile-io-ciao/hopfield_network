#include "image_processor.hpp"

#include <filesystem>

#include "functions.hpp"

namespace hp {

ImageProcessor::ImageProcessor(std::string source, std::string destination) {
  if (!std::filesystem::exists(source)) {
    throw std::runtime_error{
        "[ImageProcessor::ImageProcessor] source folder doesn't exists: " +
        source};
  }
  this->sourceFolder = source;
  this->destinationFolder = destination;
}

// funzione che elabora tutte le immagini che sono nella cartella di origine
void ImageProcessor::process() const {
  if (!std::filesystem::exists(this->destinationFolder)) {
    std::filesystem::create_directories(this->destinationFolder);
  }

  // itero su tutti i file png della cartella sorgente
  for (const auto& entry :
       std::filesystem::directory_iterator(this->sourceFolder)) {
    if (entry.is_regular_file()) {
      auto path = entry.path();
      std::string ext = path.extension().string();
      if (ext == ".png") {
        sf::Image img;
        img.loadFromFile(path.string());

        if (!img.loadFromFile(path.string())) {
          throw std::runtime_error{
              "[ImageProcessor::process] error loading image: " +
              path.string()};
        }

        sf::Image elaborated = transform(img);

        // salvo risultato in destinationFolder con lo stesso nome
        std::string outPath =
            this->destinationFolder + "/" + path.filename().string();
        elaborated.saveToFile(outPath);
      }
    }
  }
}

ImageResized::ImageResized(std::string source, std::string destination)
    : ImageProcessor(source, destination) {}

sf::Image ImageResized::transform(const sf::Image& input) const {
  std::vector<sf::Color> colors = vector_from_image(input);
  std::vector<int> vector1 = blacknwhite(colors);
  int width = input.getSize().x;
  int height = input.getSize().y;
  std::vector<int> v_interpolated =
      bilinear_interpolation(vector1, width, height);
  sf::Image image = image_from_vector(v_interpolated);
  return image;
}

ImageZoomed::ImageZoomed(std::string source, std::string destination)
    : ImageProcessor(source, destination) {}

sf::Image ImageZoomed::transform(const sf::Image& input) const {
  std::vector<sf::Color> colori = vector_from_image(input);
  std::vector<int> vector1 = blacknwhite(colori);
  std::vector<int> vector2 = zoom(vector1);
  sf::Image image = image_from_vector(vector2);
  return image;
}

ImageNoised::ImageNoised(std::string source, std::string destination)
    : ImageProcessor(source, destination) {}

sf::Image ImageNoised::transform(const sf::Image& input) const {
  std::vector<sf::Color> colors = vector_from_image(input);
  std::vector<int> vector1 = blacknwhite(colors);
  std::vector<int> vector2 = noise(vector1);
  std::vector<int> vector3 = vertical_cut(vector2);
  std::vector<int> vector4 = orizontal_cut(vector3);
  sf::Image image = image_from_vector(vector4);
  return image;
}
}  // namespace hp