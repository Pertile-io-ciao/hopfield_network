#ifndef HEBB_HPP
#define HEBB_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace hp {
  class Hebb {
   public:
    Hebb(std::string source, std::string destination);
    void process();
    void save_matrix(const std::vector<std::vector<float>>& matrix) const;

   private:
    std::string sourceFolder;
    std::string destinationFolder;

    // spostata qui per non ridefinirla dentro process()
    //void save_matrix(const std::vector<std::vector<float>>& matrix) const;
     // dichiarazione friend: permette al test di accedere ai membri privati
    //friend void test_save_matrix_accessible();
  };
}
#endif
