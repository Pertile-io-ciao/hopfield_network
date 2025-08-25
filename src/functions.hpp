#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace hp {

std::vector<sf::Color> vector_from_image(const sf::Image& image);
std::vector<int> blacknwhite(const std::vector<sf::Color>& v);
std::vector<int> bilinear_interpolation(const std::vector<int>& input,
                                        int width, int height);
std::vector<int> zoom(const std::vector<int>& v, int zoom_factor = 4);
sf::Image image_from_vector(const std::vector<int>& bwvector);

std::vector<std::vector<float>> hebb(const std::vector<std::vector<int>>& v);
std::vector<int> neuron_update(int i, const std::vector<int>& x,
                               const std::vector<std::vector<float>>& W);
float energy_function(const std::vector<int>& x,
                      const std::vector<std::vector<float>>& W);

std::vector<int> noise(std::vector<int> v, float prob = 0.08f);
std::vector<int> orizontal_cut(std::vector<int> v, int width = 10); //widht max 14
std::vector<int> vertical_cut(std::vector<int> v, int width = 10);  //widht max 14
}  // namespace hp
#endif