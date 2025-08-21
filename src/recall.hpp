#ifndef RECALL_HPP
#define RECALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class Recall {
  std::string matrix_Folder;
  std::string image_Folder;

  std::vector<std::vector<float>> weight_matrix;
  float energy;
  std::vector<int> current_pattern;

  std::vector<std::vector<float>> load_matrix();

 public:
  Recall(const std::string& matrix_path);

  void initialize_from_image(const std::string& image_file);

  void update(int i);

  bool is_converged() const;
  int pattern_side() const;
  float get_energy() const;

  const std::vector<int>& get_pattern_ref() const;
  const std::vector<uint8_t>& get_rgba_buffer_ref() const;
};

#endif