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
std::vector<int> noise(std::vector<int> v, float prob = 0.08f);

std::vector<std::vector<int>> hebb(const std::vector<std::vector<int>>& v);