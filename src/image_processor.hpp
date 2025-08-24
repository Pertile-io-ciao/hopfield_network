#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace hp{

class ImageProcessor {
 public:
  std::string sourceFolder;
  std::string destinationFolder;

  ImageProcessor(std::string source, std::string destination);
  void process() const;

  /**
   * Metodo specifico di trasformazione virtuale
   */
  virtual sf::Image transform(const sf::Image& input) const = 0;
};

// classe per il ridimensionamento dell'immagine
class ImageResized : public ImageProcessor {
 public:
  ImageResized(std::string source, std::string destination);
  sf::Image transform(const sf::Image& input) const override;
};

// classe per lo Zoom dell'immagine
class ImageZoomed : public ImageProcessor {
 public:
  ImageZoomed(std::string source, std::string destination);
  sf::Image transform(const sf::Image& input) const override;
};

class ImageNoised : public ImageProcessor {
 public:
  ImageNoised(std::string source, std::string destination);
  sf::Image transform(const sf::Image& input) const override;
};

}

#endif
