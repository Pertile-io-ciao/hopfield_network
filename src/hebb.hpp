#ifndef HEBB_HPP
#define HEBB_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class Hebb {
 public:
  Hebb(std::string source, std::string destination);
  void process();
  
 private:
  std::string sourceFolder;
  std::string destinationFolder;

  
  // spostata qui per non ridefinirla dentro process()
  void save_matrix(const std::vector<std::vector<float>>& matrix) const;
};

#endif
