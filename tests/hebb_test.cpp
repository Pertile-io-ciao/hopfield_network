#include "../src/hebb.hpp"

#include <cstdio>  // per remove_all
#include <filesystem>
#include <fstream>

#include "doctest.h"

namespace fs = std::filesystem;
using namespace hp;

TEST_CASE("Hebb::save_matrix - normal operation") {
  // setup
  std::string testFolder = "test_output";
  std::filesystem::remove_all(
      testFolder);         // pulizia iniziale da run precedenti
  Hebb h("", testFolder);  // cartella sorgente non usata

  std::vector<std::vector<float>> matrix = {{1.1234567f, 2.7654321f},
                                            {3.1f, 4.2f}};

  // azione + verifica che non lanci eccezioni
  CHECK_NOTHROW(h.save_matrix(matrix));

  // verifica che il file sia stato creato
  std::filesystem::path file =
      std::filesystem::path(testFolder) / "weight_matrix.txt";
  CHECK(std::filesystem::exists(file));

  // verifica contenuto file
  std::ifstream in(file);
  double val1, val2;

  // prima riga
  in >> val1 >> val2;
  CHECK(val1 == doctest::Approx(1.123457));
  CHECK(val2 == doctest::Approx(2.765432));

  // seconda riga
  in >> val1 >> val2;
  CHECK(val1 == doctest::Approx(3.100000));
  CHECK(val2 == doctest::Approx(4.200000));
  // cleanup
  std::filesystem::remove_all(testFolder);
}

TEST_CASE("Hebb::save_matrix - empty matrix") {
  Hebb h("", "test_output");
  std::vector<std::vector<float>> matrix;

  CHECK_THROWS_AS(h.save_matrix(matrix), std::runtime_error);
} 

TEST_CASE("Hebb::save_matrix - empty row in matrix") {
  Hebb h("", "test_output");
  std::vector<std::vector<float>> matrix = {{1.0f, 2.0f}, {}};

  CHECK_THROWS_AS(h.save_matrix(matrix), std::runtime_error);
}  // Atteso runtime_error (messaggio “Found an empty row in the matrix!”).

TEST_CASE("Hebb::save_matrix - cannot create directory") {
  Hebb h("", "/root/forbidden_matrix");
  std::vector<std::vector<float>> matrix = {{1.0f, 2.0f}};

  CHECK_THROWS_AS(h.save_matrix(matrix), std::runtime_error);
}
// Atteso runtime_error quando tenta create_directories o apertura file

TEST_CASE("Hebb::process - source folder does not exist") {
  Hebb h("nonexistent_folder", "out");
  CHECK_THROWS_AS(h.process(), std::runtime_error);
}  // Verifichi che arrivi runtime_error.

TEST_CASE("Hebb::process - empty source folder") {
  fs::create_directory("empty_src");
  Hebb h("empty_src", "out");
  CHECK_THROWS_WITH(h.process(), doctest::Contains("no patterns found"));
  fs::remove_all("empty_src");
}  // Crea una cartella vuota.
// Iterazione non trova file → dopo il loop patterns è vuoto → lancia "no
// patterns found". Usi CHECK_THROWS_WITH(..., doctest::Contains("...")) per
// controllare sottostringa del messaggio.

TEST_CASE("Hebb::process - non-png file in folder") {
  fs::create_directory("src_with_txt");
  std::ofstream("src_with_txt/file.txt") << "test";
  Hebb h("src_with_txt", "out");
  CHECK_THROWS_WITH(h.process(), doctest::Contains("non-png file"));
  fs::remove_all("src_with_txt");
}  // Verifichi che il messaggio contenga “non-png file”.

TEST_CASE("Hebb::process - corrupted PNG") {
  fs::create_directory("src_with_bad_png");
  std::ofstream("src_with_bad_png/bad.png") << "not_a_real_png";
  Hebb h("src_with_bad_png", "out");
  CHECK_THROWS_WITH(h.process(), doctest::Contains("failed to load image"));
  fs::remove_all("src_with_bad_png");
}