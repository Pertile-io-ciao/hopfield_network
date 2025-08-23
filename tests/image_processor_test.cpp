#include "doctest.h"
#include "../src/image_processor.hpp"
#include <filesystem>
#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;
 
//non posso fare testcase su image processor in quanto è una classe astratta
TEST_CASE("ImageResized - transform and process") {
    fs::create_directory("test_input");
    sf::Image img;
    img.create(2, 2, sf::Color::Black);
    img.saveToFile("test_input/test.png");
    fs::create_directory("test_output");

    hp::ImageResized processor("test_input", "test_output");
    sf::Image out = processor.transform(img);
    CHECK(out.getSize().x > 0);
    CHECK(out.getSize().y > 0);

    CHECK_NOTHROW(processor.process());
    CHECK(fs::exists("test_output/test.png"));

    fs::remove_all("test_input");
    fs::remove_all("test_output");
}

TEST_CASE("ImageZoomed - transform produces image") {
    fs::create_directory("test_input");
    sf::Image img;
    img.create(2, 2, sf::Color::White);
    img.saveToFile("test_input/test.png");
    fs::create_directory("test_output");

    hp::ImageZoomed processor("test_input", "test_output");
    sf::Image out = processor.transform(img);
    CHECK(out.getSize().x > 0);
    CHECK(out.getSize().y > 0);

    CHECK_NOTHROW(processor.process());
    CHECK(fs::exists("test_output/test.png"));

    fs::remove_all("test_input");
    fs::remove_all("test_output");
}

TEST_CASE("ImageNoised - transform and process") {
    fs::create_directory("test_input");
    sf::Image img;
    img.create(2, 2, sf::Color::White);
    img.saveToFile("test_input/test.png");
    fs::create_directory("test_output");

    hp::ImageNoised processor("test_input", "test_output");
    sf::Image out = processor.transform(img);
    CHECK(out.getSize().x > 0);
    CHECK(out.getSize().y > 0);

    CHECK_NOTHROW(processor.process());
    CHECK(fs::exists("test_output/test.png"));

    fs::remove_all("test_input");
    fs::remove_all("test_output");
}
