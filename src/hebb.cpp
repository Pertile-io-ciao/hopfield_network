#include "hebb.hpp"

#include <filesystem> //lavora con percorsi e cartelle
#include <fstream> // legge/scrive file
// <iostream> e altre son già incluse in hebb.hpp
#include "functions.hpp"

Hebb::Hebb(std::string source, std::string destination) {
  this->sourceFolder = source;  // salva le cartelle sorgente
  this->destinationFolder = destination; 
}

/*
void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {
  namespace fs = std::filesystem;

  if (!fs::exists(this->destinationFolder)) {
    fs::create_directories(this->destinationFolder);
  }  //crea la cartella destinazione se non esiste

  fs::path outpath =  fs::path(this->destinationFolder) /  "weight_matrix.txt";
// percorso completo del file weight_matrix.txt

  std::ofstream out(outpath, std::ios::out);  //apre file di scrittura out
  if (!out) {
    throw std::runtime_error(
        "Error: impossible to open file data/weight_matrix.txt");  // genera errore se il file non può essere aperto (chatgpt)
  }  //in questo caso diamo per ovvio che si vada in data
  out << std::fixed << std::setprecision(6);  //precisione dei float in decimi

  for (const auto& row : matrix) {  
    for (float value : row) {
      out << value << " ";
    }
    out << "\n";
  }
  out.close();
}
*/

void Hebb::save_matrix(const std::vector<std::vector<float>>& matrix) const {

    std::filesystem::path outdir(destinationFolder);  //rappresenta la cartella di destinazione
    std::error_code ec; //cattura eventuali errori
    
    // crea la cartella di destinazione se manca
    if (!std::filesystem::exists(outdir)) {
        std::filesystem::create_directories(outdir, ec);
        if (ec) { //Se c’è stato un errore nella creazione della cartella, stampa un messaggio e lancia un’eccezione.
            std::string msg = "[Hebb::save_matrix] cannot create directory '" + outdir.string()
                              + "': " + ec.message();
            std::cerr << msg << '\n';
            throw std::runtime_error(msg);
        }
    }

    // percorso file completo
    std::filesystem::path outpath = outdir / "weight_matrix.txt";

    // apri file in testo (per file molto grandi preferisci binario)
    std::ofstream out(outpath, std::ios::out);
    //se il file non si apre stampa errore e lancia eccezione
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

    // formattazione leggibile a 6 numeri decimali
    out << std::fixed << std::setprecision(6);

    // scrivo ogni riga, valori separati da spazio
    for (const auto& row : matrix) {
        // se la riga è vuota scrive vuota
        if (row.empty()) {
            out << '\n';
            continue;
        } //scrive i valori delle righe separati da spazi
        for (size_t j = 0; j < row.size(); ++j) {
            if (j) out << ' ';
            out << row[j];
        }
        out << '\n';
    }

    out.close();
    //problemi durante la scrittura
    if (!out) {
        std::cerr << "[Hebb::save_matrix] warning: problem occurred while writing to " << outpath << '\n';
        throw std::runtime_error("Error writing weight matrix to file");
    }

 // Stampa un messaggio di successo con le dimensioni della matrice e il percorso del file.
    std::cout << "[Hebb::save_matrix] saved matrix to: " << outpath
              << " (" << matrix.size() << " x " << matrix[0].size() << ")\n";
}


/*
// funzione che elabora tutte le immagini che sono nella cartella di origine
void Hebb::process() { 
  std::vector<std::vector<int>> patterns;

  if (!std::filesystem::exists(this->destinationFolder)) {
    std::filesystem::create_directories(this->destinationFolder);
  }

  for (const auto& entry : //entry rappresenta ogni elemento trovato nella cartella sorgente
       std::filesystem::directory_iterator(this->sourceFolder)) {
    
    if (entry.is_regular_file()) { //regular = non cartella o link
      auto path = entry.path();  // path=percorso
      std::string ext = path.extension().string(); //salva l'estensione del file come stringa
      if (ext == ".png") {
        sf::Image img;
        if (!img.loadFromFile(path.string())) {
          std::cerr << "Failed to load image: " << path << "\n";
          continue; //se fallisce salta al file successivo
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
*/

void Hebb::process() {
    //using namespace std; //permette di usare oggetti come vector, cout, cerr senza scrivere std::
    namespace fs = std::filesystem;

    // 1) collect patterns
    std::vector<std::vector<int>> patterns; //vettore di vettori
    //controllo cartella sorgente
    if (!fs::exists(sourceFolder)) {
        std::cerr << "[Hebb::process] ERROR: sourceFolder does not exist: " << sourceFolder << "\n";
    } else {
        for (const auto& entry : fs::directory_iterator(sourceFolder)) { //scorre tutti i file
            if (!entry.is_regular_file()) continue;
            auto path = entry.path();
            //std::cout << "[Hebb::process] found file: " << path << "\n";
            // optionally accept .png and .PNG:
            auto ext = path.extension().string();
            // normalize extension lower-case
            for (auto &c : ext) c = std::tolower((unsigned char)c);
            if (ext != ".png") {
                std::cout << "[Hebb::process] skipping non-png: " << path.filename() << "\n";
                continue;
            }

            sf::Image img;
            if (!img.loadFromFile(path.string())) {
                std::cerr << "[Hebb::process] WARN: loadFromFile failed for " << path << "\n";
                continue;
            }
            //cout << "[Hebb::process] loaded image " << path.filename() << " size: "
          //       << img.getSize().x << "x" << img.getSize().y << "\n";

            auto colors = vector_from_image(img); // verifica firma
            //std::cout << "[Hebb::process] vector_from_image size: " << colors.size() << "\n";

            auto pattern = blacknwhite(colors);  // verifica firma, -1/+1
            //cout << "[Hebb::process] blacknwhite produced length: " << pattern.size() << "\n";

            if (pattern.empty()) { //se il pattern è vuoto lo ignora
                std::cerr << "[Hebb::process] WARN: empty pattern for " << path.filename() << "\n";
                continue;
            }
            patterns.push_back(std::move(pattern));
        }
    }

   //std::cout << "[Hebb::process] total patterns collected: " << patterns.size() << "\n";

    if (patterns.empty()) {
        std::cerr << "[Hebb::process] No patterns -> aborting process()\n";
        return;
    } //non ha trovato patterns

    // 2) compute matrix (wrap hebb(...) call and catch)
    std::vector<std::vector<float>> W;
    try {
        //std::cout << "[Hebb::process] calling hebb(...) with " << patterns.size() << " patterns\n";
        W = hebb(patterns); // funzione definita in functions.cpp
    } catch (const std::exception& e) {
        std::cerr << "[Hebb::process] ERROR: hebb(...) threw: " << e.what() << "\n";
        return;
    } catch (...) { //catch generico, intercetta tutto
        std::cerr << "[Hebb::process] ERROR: hebb(...) threw unknown\n";
        return;
    }

    //std::cout << "[Hebb::process] hebb returned matrix size: " << W.size()
    //     << " x " << (W.empty() ? 0 : W[0].size()) << "\n";

    if (W.empty()) {
        std::cerr << "[Hebb::process] ERROR: weight matrix empty -> aborting\n";
        return;
    }

    // quick writable test in destinationFolder
    try {
        fs::path outdir(destinationFolder);
        fs::create_directories(outdir);
        fs::path testp = outdir / "process_write_test.txt";
        std::ofstream t(testp); //Prova ad aprire un file in scrittura (ofstream) nel percorso testp.
        if (!t.is_open()) {
            std::cerr << "[Hebb::process] ERROR: cannot open test file in destination: " << testp << "\n";
        } else {
            //t << "ok\n";
            t.close();
            //std::cout << "[Hebb::process] wrote test file: " << testp << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "[Hebb::process] WARNING: could not write test file: " << e.what() << "\n";
    }

    // 3) save matrix (with try/catch)
    try {
        //std::cout << "[Hebb::process] calling save_matrix(...)\n";
        save_matrix(W);
        //std::cout << "[Hebb::process] save_matrix returned\n";
    } catch (const std::exception& e) {
        std::cerr << "[Hebb::process] save_matrix threw: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "[Hebb::process] save_matrix threw unknown exception\n";
    }

    //std::cout << "[Hebb::process] done\n";
} 