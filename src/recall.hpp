#ifndef RECALL_HPP
#define RECALL_HPP

#include <string>
#include <vector>

namespace hp {
class Recall {
 private:
  std::string matrix_Folder;
  std::string image_clicked;
  std::vector<std::vector<float>> weight_matrix;
  float energy;
  std::vector<int> current_pattern;

 public:
  Recall(const std::string& matrix_path);

  std::vector<std::vector<float>> load_matrix();

  void initialize_from_image(const std::string& image_file);

  void update(int i);
  void compute_energy();

  int pattern_side() const;
  float get_energy() const;

  const std::vector<int>& get_pattern_ref() const;
};
}  // namespace hp
#endif