#include "../src/image_processor.hpp"

#include <SFML/Graphics.hpp>
#include <filesystem>

#include "doctest.h"

// non posso fare testcase su image processor in quanto è una classe astratta
TEST_CASE("ImageResized - transform and process") {
  std::filesystem::create_directory("test_input");
  sf::Image img;
  img.create(2, 2, sf::Color::Black);
  img.saveToFile("test_input/test.png");
  std::filesystem::create_directory("test_output");

  hp::ImageResized processor("test_input", "test_output");
  sf::Image out = processor.transform(img);
  CHECK(out.getSize().x > 0);
  CHECK(out.getSize().y > 0);

  CHECK_NOTHROW(processor.process());
  CHECK(std::filesystem::exists("test_output/test.png"));

  std::filesystem::remove_all("test_input");
  std::filesystem::remove_all("test_output");
}

TEST_CASE("ImageZoomed - transform produces image") {
  std::filesystem::create_directory("test_input");
  sf::Image img;
  img.create(2, 2, sf::Color::White);
  img.saveToFile("test_input/test.png");
  std::filesystem::create_directory("test_output");

  hp::ImageZoomed processor("test_input", "test_output");
  sf::Image out = processor.transform(img);
  CHECK(out.getSize().x > 0);
  CHECK(out.getSize().y > 0);

  CHECK_NOTHROW(processor.process());
  CHECK(std::filesystem::exists("test_output/test.png"));

  std::filesystem::remove_all("test_input");
  std::filesystem::remove_all("test_output");
}

TEST_CASE("ImageNoised - transform and process") {
  std::filesystem::create_directory("test_input");
  sf::Image img;
  img.create(2, 2, sf::Color::White);
  img.saveToFile("test_input/test.png");
  std::filesystem::create_directory("test_output");

  hp::ImageNoised processor("test_input", "test_output");
  sf::Image out = processor.transform(img);
  CHECK(out.getSize().x > 0);
  CHECK(out.getSize().y > 0);

  CHECK_NOTHROW(processor.process());
  CHECK(std::filesystem::exists("test_output/test.png"));

  std::filesystem::remove_all("test_input");
  std::filesystem::remove_all("test_output");
}
