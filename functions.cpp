#include "functions.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
// lato immagine
int l = 64;

// zoom viene passato come parametro
// int n= 5;

std::vector<sf::Color> vector_from_image(const sf::Image& image) {
  int width = image.getSize().x;
  int height = image.getSize().y;

  std::vector<sf::Color> result;
  result.reserve(width * height);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      result.push_back(image.getPixel(x, y));
    }
  }

  return result;
}
// mi trovo un vettore composto da tutti sf::Color che sono praticamente i pixel

std::vector<int> blacknwhite(const std::vector<sf::Color>& v) {
  std::vector<int> result;

  for (std::size_t i = 0; i < v.size(); ++i) {
    const sf::Color& c = v[i];

    // Ignora completamente i pixel trasparenti (opzionale)
    if (c.a < 10) {
      result.push_back(-1);  // considera come bianco
      continue;
    }

    // Calcola la luminanza percepita
    double luminanza = (0.299 * c.r + 0.587 * c.g + 0.114 * c.b);
    int h = (luminanza < 127) ? 1 : -1;
    result.push_back(h);
  }

  return result;
}
std::vector<int> bilinear_interpolation(const std::vector<int>& input, int inW,
                                        int inH) {
  std::vector<int> output(l * l);

  for (int y = 0; y < l; ++y) {
    for (int x = 0; x < l; ++x) {
      float gx = ((x + 0.5f) * inW) / l - 0.5f;
      float gy = ((y + 0.5f) * inH) / l - 0.5f;

      int x0 = std::floor(gx);
      int y0 = std::floor(gy);
      int x1 = std::min(x0 + 1, inW - 1);
      int y1 = std::min(y0 + 1, inH - 1);

      x0 = std::clamp(x0, 0, inW - 1);
      y0 = std::clamp(y0, 0, inH - 1);

      float dx = gx - x0;
      float dy = gy - y0;

      auto at = [&](int x, int y) -> float {
        return (input[y * inW + x] > 0) ? 1.0f : 0.0f;
      };

      float v00 = at(x0, y0);
      float v10 = at(x1, y0);
      float v01 = at(x0, y1);
      float v11 = at(x1, y1);

      float value = v00 * (1 - dx) * (1 - dy) + v10 * dx * (1 - dy) +
                    v01 * (1 - dx) * dy + v11 * dx * dy;

      output[y * l + x] = (value < 0.5f) ? -1 : 1;
    }
  }

  return output;
}

std::vector<int> zoom(const std::vector<int>& v, int n) {
  int newL = l * n;
  std::vector<int> result(newL * newL);

  for (int y = 0; y < l; ++y) {
    for (int x = 0; x < l; ++x) {
      int value = v[y * l + x];

      // Scrivi valore in blocco n x n
      for (int dy = 0; dy < n; ++dy) {
        for (int dx = 0; dx < n; ++dx) {
          int newX = x * n + dx;
          int newY = y * n + dy;
          result[newY * newL + newX] = value;
        }
      }
    }
  }

  return result;
}

// in input ho vettore di pixel bn di immagine quadrata quindi il lato è radice
// di size
sf::Image image_from_vector(const std::vector<int>& dates) {
  sf::Image image;
  int lato = (int)sqrt(dates.size());
  image.create(lato, lato);

  for (int y = 0; y < lato; ++y) {
    for (int x = 0; x < lato; ++x) {
      int value = dates[y * lato + x];
      sf::Color color = (value == 1) ? sf::Color::Black : sf::Color::White;
      image.setPixel(x, y, color);
    }
  }

  return image;
}

sf::Image image_from_vector(const std::vector<int>& dates,
                            const sf::Image& image) {
  int width = image.getSize().x;
  int height = image.getSize().y;

  sf::Image imagebw;
  imagebw.create(width, height);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int value = dates[y * width + x];
      sf::Color color = (value == 1) ? sf::Color::Black : sf::Color::White;
      imagebw.setPixel(x, y, color);
    }
  }

  return imagebw;
}
