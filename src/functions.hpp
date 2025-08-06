#include <SFML/Graphics.hpp>
#include <vector>

struct pixel {
  double r;
  double g;
  double b;
};

std::vector<sf::Color> vector_from_image(const sf::Image& image);
std::vector<int> blacknwhite(const std::vector<sf::Color>& v);
std::vector<int> bilinear_interpolation(const std::vector<int>& input, int inW,
                                        int inH);
sf::Image image_from_vector(const std::vector<int>& dates);
sf::Image image_from_vector(const std::vector<int>& dates,
                            const sf::Image& image);
std::vector<int> zoom(const std::vector<int>& v, int n);

std::vector<std::vector<int>> hebb(const std::vector<std::vector<int>>& v);
void save_matrix(const std::vector<std::vector<int>>& matrix);
std::vector<std::vector<int>> load_matrix();

std::vector<int> hopfield_update(const std::vector<int>& x,
                                 const std::vector<std::vector<double>>& W);
double energy_function(const std::vector<int>& x,
                       const std::vector<std::vector<double>>& W);

std::vector<int> noise(std::vector<int> v, float prob = 0.08f);
std::vector<int> orizontal_cut(std::vector<int> v, int side_lenght = 64, int start = 4, int end = 12);
std::vector<int> vertical_cut(std::vector<int> v, int side_lenght = 64, int start = 57, int end = 61);
