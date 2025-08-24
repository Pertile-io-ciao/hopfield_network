#ifndef HEBB_HPP
#define HEBB_HPP

#include <string>
#include <vector>

namespace hp {
class Hebb {
 private:
  std::string sourceFolder;
  std::string destinationFolder;

 public:
  Hebb(std::string source, std::string destination);
  void process();
  void save_matrix(const std::vector<std::vector<float>>& matrix) const;
};
}  // namespace hp
#endif
