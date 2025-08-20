#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// #include <cstdlib>
// #include <random>

#include "functions.hpp"

TEST_CASE("testing the conversion from an image into a vector") {
  // sf::Image image;
  // std::vector<sf::Color> pixels;
  // std::random_device r;
  // std::default_random_engine eng{r()};
  // std::uniform_real_distribution<sf::Uint8> dist_color{0,255};

  //  int i;
  //  sf::Color v = pixels[i];
  //  for (i=0; i != 255; ++i) {
  //    v = {dist_color(eng), dist_color(eng), dist_color(eng),
  //    dist_color(eng)};
  //  }

  SUBCASE("testing the blacknwhite function") {
    int width(12);
    int height(10);
    std::size_t size = (width * height);
    std::vector<sf::Color> pixels(size, sf::Color(0, 0, 0, 0));
    pixels[0] = {0, 0, 0, 0};          // -1
    pixels[1] = {0, 0, 0, 100};        // 1
    pixels[2] = {10, 10, 10, 100};     // 1
    pixels[10] = {10, 10, 10, 255};    // 1
    pixels[25] = {122, 39, 234, 255};  // 1
    pixels[30] = {122, 234, 39, 255};  // -1
    std::vector<int> result = blacknwhite(pixels);

    CHECK(pixels.size() == doctest::Approx(120));
    CHECK(result[0] == doctest::Approx(-1));
    CHECK(result[1] == doctest::Approx(1));
    CHECK(result[2] == doctest::Approx(1));
    CHECK(result[10] == doctest::Approx(1));
    CHECK(result[25] == doctest::Approx(1));
    CHECK(result[30] == doctest::Approx(-1));
  }

  SUBCASE("testing the bilinear interpolation function") {
    int inW{4};
    int inH{5};
    std::size_t insize = (inW * inH);
    std::vector<int> input1(insize, 1);
    std::vector<int> output1(64 * 64, 1);
    CHECK(output1 == bilinear_interpolation(input1, inW, inH));
    std::vector<int> input2(insize, 1);
    input2[5] = input2[6] = input2[9] = input2[10] = input2[13] = input2[14] =
        -1;  // immagine 4x5 bianca con bordo nero
    std::vector<int> output2 = bilinear_interpolation(input2, inW, inH);
    CHECK(output2[259] == 1); // occhio all'indice da inserire
    // AGGIUNGERE ALTRI CASI
  }

  SUBCASE("testing the zoom function") {
    int n{2};
    int inW{4};
    int inH{5};
    std::vector<int> input2(inW * inH, 1);
    input2[5] = input2[6] = input2[9] = input2[10] = input2[13] = input2[14] =
        -1;  // immagine 4x5 bianca con bordo nero
    std::vector<int> output2 = bilinear_interpolation(
        input2, inW,
        inH);  // ho costruito la stessa immagine del test precedente
    std::vector<int> zoomed2 = zoom(output2, n);
    CHECK(zoomed2[1159] == output2[259]); //1
    // AGGIUNGERE ALTRI CASI


    // VECTOR_FROM_IMAGE E IMAGE_FROM_VECTOR ANCORA DA TESTARE

  }
}

TEST_CASE("testing the hebb rule"){
  std::vector<int> im1{-1, 1, 1, -1};
  std::vector<int> im2{1, -1, -1, 1};
  std::vector<std::vector<int>> patterns = {im1, im2};
  std::vector<std::vector<float>> weight_matrix = hebb(patterns);
/*
  SUBCASE("testing the vector_of_vectors function"){
    std::vector<std::vector<int>> patterns1 = vector_of_vectors(im1);
    std::vector<std::vector<int>> patterns1 = vector_of_vectors(im2);
    CHECK(patterns1 == patterns);
  }
*/

  SUBCASE("testing the hebb function") {
  CHECK(weight_matrix[0][0] == 0.0f);
  CHECK(weight_matrix[1][1] == weight_matrix[0][0]);
  CHECK(weight_matrix[0][1] == -0.5f);
  CHECK(weight_matrix[1][0] == weight_matrix[0][1]);
  CHECK(weight_matrix[1][2] == 0.5f);
  CHECK(weight_matrix[2][1] == weight_matrix[1][2]);
  }

  SUBCASE("testing the hopfield_update function") {
    std::vector<int> x{1, -1, 1, -1};
    auto new_x = neuron_update(x, weight_matrix);
    CHECK(new_x[0] == -1);
    CHECK(new_x[1] == 1);
    CHECK(new_x[2] == 1);
    CHECK(new_x[3] == -1);
  }
}


