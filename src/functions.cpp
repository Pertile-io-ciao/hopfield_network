#include "functions.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>    // per time()
#include <fstream>  //per load matrix
#include <iostream>
#include <sstream>  //per load matrix
#include <stdexcept>
#include <string>
#include <vector>

// lato immagine
int l = 56;  // occhio alle variabili globali

std::vector<sf::Color> vector_from_image(const sf::Image& image) {
  int width = image.getSize().x;
  int height = image.getSize().y;

  std::vector<sf::Color> result;
  result.reserve(
      width * height);  // metodo che serve a dare una nuova taglia al vettore

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      result.push_back(image.getPixel(x, y));
    }
  }

  return result;
}
// mi trovo un vettore composto da tutti sf::Color che sono praticamente i pixel

std::vector<int> blacknwhite(const std::vector<sf::Color>& v) {
  if (v.empty()) {
    throw std::runtime_error{"[blacknwhite] empty std::vector<int>"};
  }
  std::vector<int> result;

  for (std::size_t i = 0; i < v.size(); ++i) {
    const sf::Color& c = v[i];

    // Ignora completamente i pixel trasparenti (opzionale)
    if (c.a < 10) {
      result.push_back(-1);  // considera come bianco
      continue;
    }

    // Calcola la luminanza percepita
    double luminance = (0.299 * c.r + 0.587 * c.g + 0.114 * c.b);
    int h = (luminance < 127) ? 1 : -1;
    result.push_back(h);
  }

  return result;
}

std::vector<int> bilinear_interpolation(const std::vector<int>& input,
                                        int width, int height) {
  if ((size_t)(width * height) != input.size()) {
    throw std::runtime_error{
        "[bilinear_interpolation] input size != widht*height"};
  }
  std::vector<int> output(l * l);
  for (int y = 0; y < l; ++y) {
    for (int x = 0; x < l; ++x) {
      float gx = ((x + 0.5f) * width) / l -
                 0.5f;  // coordinate dei pixel nell'immagine originale
      float gy = ((y + 0.5f) * height) / l - 0.5f;  //

      int x0 = std::floor(gx);
      int y0 = std::floor(gy);
      int x1 = std::min(x0 + 1, width - 1);
      int y1 = std::min(y0 + 1, height - 1);

      x0 = std::clamp(x0, 0, width - 1);
      y0 = std::clamp(y0, 0, height - 1);

      float dx = gx - x0;
      float dy = gy - y0;

      auto at = [&](int x, int y) -> float {
        return (input[y * width + x] > 0) ? 1.0f : 0.0f;
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

std::vector<int> zoom(const std::vector<int>& v, int zoom_factor) {
  if ((size_t)(l * l) != v.size()) {
    throw std::runtime_error{"[zoom] input size!= l*l."};
  }
  int newL = l * zoom_factor;
  std::vector<int> result(newL * newL);

  for (int y = 0; y < l; ++y) {
    for (int x = 0; x < l; ++x) {
      int value = v[y * l + x];

      // Scrivi valore in blocco n x n
      for (int dy = 0; dy < zoom_factor; ++dy) {
        for (int dx = 0; dx < zoom_factor; ++dx) {
          int newX = x * zoom_factor + dx;
          int newY = y * zoom_factor + dy;
          result[newY * newL + newX] = value;
        }
      }
    }
  }

  return result;
}

// in input ho vettore di pixel bn di immagine quadrata quindi il lato è radice
// di size
sf::Image image_from_vector(const std::vector<int>& bwvector) {
  
  sf::Image image;
  int side = (int)sqrt(bwvector.size());
   if (side * side != (int)bwvector.size()) {
    throw std::invalid_argument{"[image_from_vector] bw image is not a perfet square"};
  }
  image.create(side, side);

  for (int y = 0; y < side; ++y) {
    for (int x = 0; x < side; ++x) {
      int value = bwvector[y * side + x];
      sf::Color color = (value == 1) ? sf::Color::Black : sf::Color::White;
      image.setPixel(x, y, color);
    }
  }

  return image;
}

std::vector<int> noise(std::vector<int> v, float prob) {
  std::vector<int> result = v;

  std::srand(static_cast<unsigned>(std::time(0)));

  for (size_t i = 0; i < result.size(); ++i) {
    float casual = static_cast<float>(std::rand()) / RAND_MAX;
    if (casual < prob) {
      if (result[i] == 1) {
        result[i] = -1;
      } else {
        result[i] = 1;
      }
    }
  }

  return result;
}

std::vector<int> vertical_cut(std::vector<int> v, int start, int end) {
  if (start < 0 || end > l || start > end) {
    throw std::runtime_error{"[vertical_cut] start and end indices are out of range"};
  }
  for (int i = 0; i < l; ++i) {
    for (int j = 0; j < l; ++j) {
      if (j >= start && j <= end) {
        v[i * l + j] = 1;
      }
    }
  }
  return v;
}  

std::vector<int> orizontal_cut(std::vector<int> v, int start, int end) {
  if (start < 0 || end > l || start > end) {
    throw std::runtime_error{"[orizontal_cut] start and end indices are out of range"};
  }
  for (int i = 0; i < l; ++i) {
    for (int j = 0; j < l; ++j) {
      if (i >= start && i <= end) {
        v[i * l + j] = 1;
      }
    }
  }
  return v;
}

// regola di hebb per calcolare la matrice dei pesi
std::vector<std::vector<float>> hebb(
    const std::vector<std::vector<int>>& pxn) {  // v è una matrice p x n
      if (pxn.empty()) {
    throw std::runtime_error{"[hebb] pxn (matrix were each row is a pattern) is empty"};
  }
  int n_pattern = pxn.size();     // numero di pattern che voglio memorizzare
  int n_neurons = pxn[0].size();  // numero di neuroni
  std::vector<std::vector<float>> W(
      n_neurons,
      std::vector<float>(n_neurons,
                         0.0));  // inizializza la matrice n x n, tutta 0
  for (int i = 0; i < n_neurons;
       ++i) {  // due cicli per considerare ogni neurone
    for (int j = 0; j < n_neurons; ++j) {
      float sum = 0;
      for (int k = 0; k < n_pattern; ++k) {
        sum += static_cast<float>(pxn[k][i]) * static_cast<float>(pxn[k][j]);
      }  // la sommatoria della regola
      if (i != j) {
        W[i][j] = sum / static_cast<float>(n_neurons);  // regola completa
      } else {
        W[i][j] = 0.0;  // niente auto-connessioni
      }
    }
  }
  return W;
};

// regola di hopfield ma che considera un neurone specifico dato come parametro,
std::vector<int> neuron_update(int i, const std::vector<int>& x,
                               const std::vector<std::vector<float>>& W) {
  int n = x.size();
  if (i < 0 || i >= n) {
    throw std::runtime_error{"[neuron_update] neuron index out of range"};
  }
  std::vector<int> x_new = x;
  float sum = 0.0;
  for (int j = 0; j < n; ++j) {
    sum += W[i][j] * x[j];
  }
  x_new[i] =
      (sum >= 0) ? 1 : -1;  // ±1 per ogni neurone; come la funzione segno

  return x_new;
}

// funzione dell'energia con float
float energy_function(const std::vector<int>& x,
                      const std::vector<std::vector<float>>& W) {
  int n = x.size();
  if ((int)W.size() != n || (int)W[0].size() != n) {
    throw std::runtime_error{"[energy_function] W (hebb matrix) uncompatibile with input vector"};
  }
  float energy = 0.0;

  for (int i = 0; i < n; ++i) {  // solito cicletto for doppio
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        energy += W[i][j] * x[i] * x[j];
      }
    }
  }

  return -0.5 * energy;
}
