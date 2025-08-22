#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

std::vector<sf::Color> vector_from_image(const sf::Image& image);
std::vector<int> blacknwhite(const std::vector<sf::Color>& v);
std::vector<int> bilinear_interpolation(const std::vector<int>& input, int inW,
                                        int inH);
sf::Image image_from_vector(const std::vector<int>& dates);
sf::Image image_from_vector(const std::vector<int>& dates,
                            const sf::Image& image);
std::vector<int> zoom(const std::vector<int>& v, int n = 4);

std::vector<std::vector<float>> hebb(const std::vector<std::vector<int>>& v);


std::vector<int> neuron_update(int i, const std::vector<int>& x,
                               const std::vector<std::vector<float>>& W);
float energy_function(const std::vector<int>& x,
                      const std::vector<std::vector<float>>& W);

std::vector<int> noise(std::vector<int> v, float prob = 0.08f);
std::vector<int> orizontal_cut(std::vector<int> v, int start = 4,
                               int end = 7);
std::vector<int> vertical_cut(std::vector<int> v, int start = 50,
                              int end = 53);

#endif