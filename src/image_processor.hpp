#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace hp {

class ImageProcessor {
 private:
  std::string sourceFolder;
  std::string destinationFolder;

 public:
  ImageProcessor(std::string source, std::string destination);
  void process() const;

  // metodo specifico di trasformazione virtuale
  virtual sf::Image transform(const sf::Image& input) const = 0;
};

// classe per il ridimensionamento dell'immagine
class ImageResized : public ImageProcessor {
 public:
  ImageResized(std::string source, std::string destination);
  sf::Image transform(const sf::Image& input) const override;
};

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

}  // namespace hp

#endif
