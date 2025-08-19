#ifndef RECALL_HPP
#define RECALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Status { //per legare pattern ed energia
  std::vector<int> pattern; //vettore di interi
  float energy; //energie
};

class Recall {

  std::string matrix_Folder;
  std::string image_Folder;

   std::vector<std::vector<float>> load_matrix();

 public:

 Recall(std::string source, std::string destination);
 void process();

 Status update_status(int i, const Status& old_status, const std::vector<std::vector<float>>& W);




};

#endif
