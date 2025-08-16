#ifndef HEBB_HPP
#define HEBB_HPP


#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>


class Hebb {
 public:
  std::string sourceFolder;


  Hebb(std::string source);
  void creatematrix();


  /**
   * Metodo specifico di trasformazione virtuale
   */
  //sf::Image transform(const sf::Image& input);


  private:
  // spostata qui per non ridefinirla dentro process()
  void save_matrix(const std::vector<std::vector<float>>& matrix);
};




#endif
