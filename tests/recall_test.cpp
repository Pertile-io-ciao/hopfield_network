#include "doctest.h"
#include "../src/recall.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("Recall::load_matrix - file esistente e corretto") {
    fs::create_directory("test_matrix");
    std::ofstream out("test_matrix/weight_matrix.txt");
    out << "1.0 0.0 -1.0\n";
    out << "0.5 2.0 3.0\n";
    out.close();

    Recall r("test_matrix");

    auto W = r.load_matrix();
    CHECK(W.size() == 2);
    CHECK(W[0].size() == 3);
    CHECK(W[0][0] == doctest::Approx(1.0f));
    CHECK(W[1][2] == doctest::Approx(3.0f));

    fs::remove_all("test_matrix");
}

TEST_CASE("Recall::load_matrix - file mancante") {
    CHECK_THROWS_AS(Recall("nonexistent"), std::runtime_error);
}

TEST_CASE("Recall::initialize_from_image - immagine valida") {
    fs::create_directory("test_img");
    sf::Image img;
    img.create(2, 2, sf::Color::Black);
    img.saveToFile("test_img/test.png");

    // preparo un weight_matrix compatibile (4 neuroni = 2x2 immagine)
    fs::create_directory("test_matrix");
    std::ofstream out("test_matrix/weight_matrix.txt");
    out << "0 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n";
    out.close();

    Recall r("test_matrix");

    CHECK_NOTHROW(r.initialize_from_image("test_img/test.png"));

    auto &pattern = r.get_pattern_ref();
    CHECK(pattern.size() == 4); // immagine 2x2 → 4 neuroni
    CHECK(r.pattern_side() == 2);

    fs::remove_all("test_img");
    fs::remove_all("test_matrix");
}

TEST_CASE("Recall::initialize_from_image - immagine inesistente") {
    fs::create_directory("test_matrix");
    std::ofstream out("test_matrix/weight_matrix.txt");
    out << "0\n";
    out.close();

    Recall r("test_matrix");
    CHECK_THROWS_AS(r.initialize_from_image("missing.png"), std::runtime_error);

    fs::remove_all("test_matrix");
}

TEST_CASE("Recall::initialize_from_image - dimension mismatch") {
    // Creo immagine 2x2 (pattern da 4 neuroni)
    fs::create_directory("test_img");
    sf::Image img;
    img.create(2, 2, sf::Color::Black);
    img.saveToFile("test_img/test.png");

    // Creo matrice con 3 neuroni → mismatch
    fs::create_directory("test_matrix");
    std::ofstream out("test_matrix/weight_matrix.txt");
    out << "1 2 3\n";
    out.close();

    Recall r("test_matrix");
    CHECK_THROWS_WITH(r.initialize_from_image("test_img/test.png"),
                      doctest::Contains("mismatch"));

    fs::remove_all("test_img");
    fs::remove_all("test_matrix");
}

TEST_CASE("Recall::update - mantiene dimensione del pattern") {
    // immagine 2x2 → 4 neuroni
    fs::create_directory("test_img");
    sf::Image img;
    img.create(2, 2, sf::Color::Black);
    img.saveToFile("test_img/test.png");

    fs::create_directory("test_matrix");
    std::ofstream out("test_matrix/weight_matrix.txt");
    out << "0 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n";
    out.close();

    Recall r("test_matrix");
    r.initialize_from_image("test_img/test.png");

    auto before = r.get_pattern_ref().size();
    CHECK_NOTHROW(r.update(0)); // aggiorna il neurone 0
    auto after = r.get_pattern_ref().size();

    CHECK(before == after); // dimensione invariata

    fs::remove_all("test_img");
    fs::remove_all("test_matrix");
}
