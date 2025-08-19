#include "hebb.hpp"

#include <filesystem>
#include <fstream>
//#include <iostream>
//#include <string>
//#include <vector>

#include "functions.hpp"

Hebb::Hebb(std::string source, std::string destination) {
  this->sourceFolder = source;  // salva le cartelle sorgente
  this->destinationFolder = destination;
}



void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {
  
  if (!std::filesystem::exists(this->destinationFolder)) {
    std::filesystem::create_directories(this->destinationFolder);
  }

  std::filesystem::path outpath = this->destinationFolder + "/" + "weight_matrix.txt";

  std::ofstream out(outpath, std::ios::out);
  if (!out) {
    throw std::runtime_error(
        "Error: impossible to open file data/weight_matrix.txt");  // chat gpt
  }
  out << std::fixed << std::setprecision(6);  //chat
  for (const auto& row : matrix) {
    for (float value : row) {
      out << value << " ";
    }
    out << "\n";
  }
  out.close();
}

/*
void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {
    namespace fs = std::filesystem;

    // crea la cartella di destinazione se manca
    fs::path outdir(destinationFolder);
    std::error_code ec;
    if (!fs::exists(outdir)) {
        fs::create_directories(outdir, ec);
        if (ec) {
            std::string msg = "[Hebb::save_matrix] cannot create directory '" + outdir.string()
                              + "': " + ec.message();
            std::cerr << msg << '\n';
            throw std::runtime_error(msg);
        }
    }

    // percorso file
    fs::path outpath = outdir / "weight_matrix.txt";

    // apri file in testo (per file molto grandi preferisci binario)
    std::ofstream out(outpath, std::ios::out);
    if (!out.is_open()) {
        std::string msg = "[Hebb::save_matrix] cannot open file for writing: " + outpath.string();
        std::cerr << msg << '\n';
        throw std::runtime_error(msg);
    }

    // se la matrice è vuota, scriviamo comunque un messaggio e terminiamo
    if (matrix.empty()) {
        out << "# empty matrix\n";
        out.close();
        std::cout << "[Hebb::save_matrix] wrote empty matrix to: " << outpath << '\n';
        return;
    }

    // formattazione leggibile
    out << std::fixed << std::setprecision(6);

    // scrivo ogni riga, valori separati da spazio
    for (const auto& row : matrix) {
        // opzionale: protezione se le righe hanno dimensioni diverse
        if (row.empty()) {
            out << '\n';
            continue;
        }
        for (size_t j = 0; j < row.size(); ++j) {
            if (j) out << ' ';
            out << row[j];
        }
        out << '\n';
    }

    out.close();
    if (!out) {
        std::cerr << "[Hebb::save_matrix] warning: problem occurred while writing to " << outpath << '\n';
        throw std::runtime_error("Error writing weight matrix to file");
    }

    std::cout << "[Hebb::save_matrix] saved matrix to: " << outpath
              << " (" << matrix.size() << " x " << matrix[0].size() << ")\n";
}
*/

// funzione che elabora tutte le immagini che sono nella cartella di origine
void Hebb::process() {  // metodo process

  std::vector<std::vector<int>> patterns;

  if (!std::filesystem::exists(this->destinationFolder)) {
    std::filesystem::create_directories(this->destinationFolder);
  }

  for (const auto& entry :
       std::filesystem::directory_iterator(this->sourceFolder)) {
    
    if (entry.is_regular_file()) {
      auto path = entry.path();  // path=percorso
      std::string ext = path.extension().string();
      if (ext == ".png") {
        sf::Image img;
        // carico immagine
        if (!img.loadFromFile(path.string())) {
          std::cerr << "Failed to load image: " << path << "\n";
          continue;
        }
        else img.loadFromFile(path.string());

        auto colors = vector_from_image(img);
        auto pattern = blacknwhite(colors);
        patterns.push_back(pattern);
      }
    }
  }
  auto weight_matrix = hebb(patterns);
  save_matrix(weight_matrix);
}

/*
void Hebb::process() {
    using namespace std;
    namespace fs = std::filesystem;

    // info base
    cout << "[Hebb::process] start\n";
    cout << "[Hebb::process] cwd: " << fs::current_path() << "\n";
    cout << "[Hebb::process] sourceFolder: " << sourceFolder << "\n";
    cout << "[Hebb::process] destinationFolder: " << destinationFolder << "\n";

    // 1) collect patterns
    vector<vector<int>> patterns;
    if (!fs::exists(sourceFolder)) {
        cerr << "[Hebb::process] ERROR: sourceFolder does not exist: " << sourceFolder << "\n";
    } else {
        for (const auto& entry : fs::directory_iterator(sourceFolder)) {
            if (!entry.is_regular_file()) continue;
            auto path = entry.path();
            cout << "[Hebb::process] found file: " << path << "\n";
            // optionally accept .png and .PNG:
            auto ext = path.extension().string();
            // normalize extension lower-case
            for (auto &c : ext) c = std::tolower((unsigned char)c);
            if (ext != ".png") {
                cout << "[Hebb::process] skipping non-png: " << path.filename() << "\n";
                continue;
            }

            sf::Image img;
            if (!img.loadFromFile(path.string())) {
                cerr << "[Hebb::process] WARN: loadFromFile failed for " << path << "\n";
                continue;
            }
            cout << "[Hebb::process] loaded image " << path.filename() << " size: "
                 << img.getSize().x << "x" << img.getSize().y << "\n";

            auto colors = vector_from_image(img); // verifica firma
            cout << "[Hebb::process] vector_from_image size: " << colors.size() << "\n";

            auto pattern = blacknwhite(colors);  // verifica firma, -1/+1
            cout << "[Hebb::process] blacknwhite produced length: " << pattern.size() << "\n";

            if (pattern.empty()) {
                cerr << "[Hebb::process] WARN: empty pattern for " << path.filename() << "\n";
                continue;
            }
            patterns.push_back(std::move(pattern));
        }
    }

    cout << "[Hebb::process] total patterns collected: " << patterns.size() << "\n";

    if (patterns.empty()) {
        cerr << "[Hebb::process] No patterns -> aborting process()\n";
        return;
    }

    // 2) compute matrix (wrap hebb(...) call and catch)
    vector<vector<float>> W;
    try {
        cout << "[Hebb::process] calling hebb(...) with " << patterns.size() << " patterns\n";
        W = hebb(patterns); // funzione definita in functions.cpp
    } catch (const std::exception& e) {
        cerr << "[Hebb::process] ERROR: hebb(...) threw: " << e.what() << "\n";
        return;
    } catch (...) {
        cerr << "[Hebb::process] ERROR: hebb(...) threw unknown\n";
        return;
    }

    cout << "[Hebb::process] hebb returned matrix size: " << W.size()
         << " x " << (W.empty() ? 0 : W[0].size()) << "\n";

    if (W.empty()) {
        cerr << "[Hebb::process] ERROR: weight matrix empty -> aborting\n";
        return;
    }

    // quick writable test in destinationFolder
    try {
        fs::path outdir(destinationFolder);
        fs::create_directories(outdir);
        fs::path testp = outdir / "process_write_test.txt";
        ofstream t(testp);
        if (!t.is_open()) {
            cerr << "[Hebb::process] ERROR: cannot open test file in destination: " << testp << "\n";
        } else {
            t << "ok\n";
            t.close();
            cout << "[Hebb::process] wrote test file: " << testp << "\n";
        }
    } catch (const std::exception& e) {
        cerr << "[Hebb::process] WARNING: could not write test file: " << e.what() << "\n";
    }

    // 3) save matrix (with try/catch)
    try {
        cout << "[Hebb::process] calling save_matrix(...)\n";
        save_matrix(W);
        cout << "[Hebb::process] save_matrix returned\n";
    } catch (const std::exception& e) {
        cerr << "[Hebb::process] save_matrix threw: " << e.what() << "\n";
    } catch (...) {
        cerr << "[Hebb::process] save_matrix threw unknown exception\n";
    }

    cout << "[Hebb::process] done\n";
} */