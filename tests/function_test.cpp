#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/functions.hpp"

TEST_CASE("testing vector_from_image") {
    SUBCASE("2x2 image with known colors") {
        sf::Image image;
        image.create(2, 2, sf::Color::Black);

        // Settiamo manualmente i pixel
        image.setPixel(0, 0, sf::Color::Red);     
        image.setPixel(1, 0, sf::Color::Green);  
        image.setPixel(0, 1, sf::Color::Blue);    
        image.setPixel(1, 1, sf::Color::White);   

        auto result = vector_from_image(image);

        // Controllo dimensione
        CHECK(result.size() == 4);

        // Controllo ordine: riga per riga (y=0, x=0..1, poi y=1, x=0..1)
        CHECK(result[0] == sf::Color::Red);
        CHECK(result[1] == sf::Color::Green);
        CHECK(result[2] == sf::Color::Blue);
        CHECK(result[3] == sf::Color::White);
    }

    SUBCASE("empty image") {
        sf::Image image;
        image.create(0, 0, sf::Color::Black);

        auto result = vector_from_image(image);
        CHECK(result.empty());
    }
}


TEST_CASE("testing the blacknwhite function") {
    int width(12);
    int height(10);
    std::size_t size = (width * height);
    std::vector<sf::Color> pixels(size, sf::Color(0, 0, 0, 0));
    //creiamo il vettore di pixel neri ma trasparenti
    //settiamo qualche pixel specifico
    pixels[0] = {0, 0, 0, 0};          // -1
    pixels[1] = {0, 0, 0, 100};        // 1
    pixels[2] = {10, 10, 10, 100};     // 1
    pixels[10] = {10, 10, 10, 255};    // 1
    pixels[25] = {122, 39, 234, 255};  // 1
    pixels[30] = {122, 234, 39, 255};  // -1
    std::vector<int> result = blacknwhite(pixels);

    CHECK(pixels.size() == 120);  
    CHECK(result[0] == -1); 
    CHECK(result[1] == 1); 
    CHECK(result[2] == 1);
    CHECK(result[10] == 1);
    CHECK(result[25] == 1);
    CHECK(result[30] == -1);
  }
  
TEST_CASE("testing bilinear_interpolation") {

    SUBCASE("uniform white image") {
        int inW = 4, inH = 4; //dimensioni originali
        int l=56; //dimensioni finali
        std::vector<int> input(inW * inH, 1); // tutta bianca
        auto output = bilinear_interpolation(input, inW, inH);

        CHECK(output.size() == l*l);
        // Tutti dovrebbero essere 1
        for (int val : output) CHECK(val == 1);
    }

    SUBCASE("uniform black image") {
        int inW = 4, inH = 4;
        std::vector<int> input(inW * inH, -1); // tutta nera
        auto output = bilinear_interpolation(input, inW, inH); //ridimensionando implicitamente l’immagine a una dimensione l x l definita globalmente all’interno della funzione stessa

        for (int val : output) CHECK(val == -1);
    }

    SUBCASE("checkerboard 2x2") {   //una specie di mini scacchiera
        int inW = 2, inH = 2;
        int l=56;
        std::vector<int> input = {1, -1,
                                  -1, 1};
        auto output = bilinear_interpolation(input, inW, inH);

        CHECK(output.size() == l*l);
        // Controllo pixel chiave: angoli
        CHECK(output[0] == 1);                // angolo in alto a sinistra
        CHECK(output[l-1] == -1);             // angolo in alto a destra
        CHECK(output[(l-1)*l] == -1);         // angolo in basso a sinistra
        CHECK(output[l*l-1] == 1);            // angolo in basso a destra
    }
  }

  TEST_CASE("testing zoom function") {
    SUBCASE("zoom 2x2 image with n=2") {
        // Supponiamo che l=2 sia globale
        extern int l; //senza extern crasha
        l = 2;

        // Matrice 2x2:
        // 1 -1
        // -1  1
        std::vector<int> input = {
            1, -1,
           -1,  1
        };

        int n = 2;
        auto output = zoom(input, n);

        int newL = l * n;
        CHECK(output.size() == newL * newL); // Deve essere 16 (4x4)

        // Controllo che ogni valore venga replicato
        // in blocchi 2x2
        CHECK(output[0] == 1);
        CHECK(output[1] == 1);
        CHECK(output[4] == 1);
        CHECK(output[5] == 1);

        CHECK(output[2] == -1);
        CHECK(output[3] == -1);
        CHECK(output[6] == -1);
        CHECK(output[7] == -1);

        CHECK(output[8] == -1);
        CHECK(output[9] == -1);
        CHECK(output[12] == -1);
        CHECK(output[13] == -1);

        CHECK(output[10] == 1);
        CHECK(output[11] == 1);
        CHECK(output[14] == 1);
        CHECK(output[15] == 1);
    }

    SUBCASE("zoom 1x1 image with n=3") {
        extern int l;
        l = 1;

        std::vector<int> input = { -1 };

        int n = 3;
        auto output = zoom(input, n);

        int newL = l * n;
        CHECK(output.size() == 9); // 3x3
        for (auto val : output) {
            CHECK(val == -1); // tutti uguali al valore originale
        }
    }

    SUBCASE("zoom 2x2 all ones with n=1") {
        extern int l;
        l = 2;

        std::vector<int> input = { 1, 1, 1, 1 };

        int n = 1;
        auto output = zoom(input, n);

        CHECK(output == input); // con n=1 deve restituire lo stesso vettore
    }
}


TEST_CASE("testing image_from_vector") {
    SUBCASE("1x1 black pixel") {
        std::vector<int> v{1};
        sf::Image img = image_from_vector(v);

        CHECK(img.getSize().x == 1);
        CHECK(img.getSize().y == 1);

        sf::Color c = img.getPixel(0, 0);
        CHECK(c == sf::Color::Black);
    }

    SUBCASE("1x1 white pixel") {
        std::vector<int> v{-1};
        sf::Image img = image_from_vector(v);

        CHECK(img.getSize().x == 1);
        CHECK(img.getSize().y == 1);

        sf::Color c = img.getPixel(0, 0);
        CHECK(c == sf::Color::White);
    }

    SUBCASE("2x2 checkerboard") {
        // 1 = nero, -1 = bianco
        // Layout:
        //  1  -1
        // -1   1
        std::vector<int> v{
            1, -1,
           -1,  1
        };
        sf::Image img = image_from_vector(v);

        CHECK(img.getSize().x == 2);
        CHECK(img.getSize().y == 2);

        CHECK(img.getPixel(0, 0) == sf::Color::Black);
        CHECK(img.getPixel(1, 0) == sf::Color::White);
        CHECK(img.getPixel(0, 1) == sf::Color::White);
        CHECK(img.getPixel(1, 1) == sf::Color::Black);
    }

    SUBCASE("3x3 all black") {
        std::vector<int> v(9, 1); // 9 valori = 3x3
        sf::Image img = image_from_vector(v);

        CHECK(img.getSize().x == 3);
        CHECK(img.getSize().y == 3);

        for (unsigned y = 0; y < 3; ++y) {
            for (unsigned x = 0; x < 3; ++x) {
                CHECK(img.getPixel(x, y) == sf::Color::Black);
            }
        }
    }

    SUBCASE("3x3 all white") {
        std::vector<int> v(9, -1);
        sf::Image img = image_from_vector(v);

        CHECK(img.getSize().x == 3);
        CHECK(img.getSize().y == 3);

        for (unsigned y = 0; y < 3; ++y) {
            for (unsigned x = 0; x < 3; ++x) {
                CHECK(img.getPixel(x, y) == sf::Color::White);
            }
        }
    }
}

TEST_CASE("testing noise function") {
  /*per testare questa funzione si cambia la probabilità dei pixel di essere girati
  si considera che con una probabilità pari a 0 non c'è cambiamento
  pari a 1 il cambiamento è totale  e   pari a 0.5 si cambia circa la metà*/
    SUBCASE("prob = 0.0 -> no change expected") {
        std::vector<int> v{1, -1, 1, -1};
        auto result = noise(v, 0.0f);
        CHECK(result == v);  // mai cambia
    }

    SUBCASE("prob = 1.0 -> all values flipped") {
        std::vector<int> v{1, -1};
        auto result = noise(v, 1.0f);
        CHECK(result[0] == -1);
        CHECK(result[1] == 1);
    }

    SUBCASE("prob = 0.5 -> about half should flip (statistical)") {
        std::vector<int> v(1000, 1);
        auto result = noise(v, 0.5f);

        int flipped = 0;
        for (size_t i = 0; i < v.size(); i++) {
            if (v[i] != result[i]) flipped++;
        }

        CHECK(flipped > 400); // accetto intervallo largo
        CHECK(flipped < 600);
    }
}
/*
TEST_CASE("testing vertical_cut") {
  extern int l;
  l=10;
    std::vector<int> v(l*l, -1);  // inizializzo tutta l'immagine a -1

    SUBCASE("cut in the middle columns") {
        int start = 1;
        int end = 2;
        auto result = vertical_cut(v, start, end);

        for (int i = 0; i < l; ++i) {
            for (int j = 0; j < l; ++j) {
                if (j >= start && j <= end)
                    CHECK(result[i*l + j] == 1);
                else
                    CHECK(result[i*l + j] == -1);
            }
        }
    }

    SUBCASE("cut entire columns") {
        auto result = vertical_cut(v, 0, l-1); //start = 0 ; end = l-1
        for (int val : result)
            CHECK(val == 1);
    }
}

TEST_CASE("testing orizontal_cut") {
  extern int l;
  l=10;
    std::vector<int> v(l*l, -1);

    SUBCASE("cut in the middle rows") {
        int start = 1;
        int end = 2;
        auto result = orizontal_cut(v, start, end);

        for (int i = 0; i < l; ++i) {
            for (int j = 0; j < l; ++j) {
                if (i >= start && i <= end)
                    CHECK(result[i*l + j] == 1);
                else
                    CHECK(result[i*l + j] == -1);
            }
        }
    }

    SUBCASE("cut entire rows") {
        auto result = orizontal_cut(v, 0, l-1);
        for (int val : result)
            CHECK(val == 1);
    }
}*/

TEST_CASE("testing the hebb rule"){
  std::vector<int> im1{-1, 1, 1, -1};
  std::vector<int> im2{1, -1, -1, 1};
  std::vector<std::vector<int>> patterns = {im1, im2};
  std::vector<std::vector<float>> weight_matrix = hebb(patterns);

  SUBCASE("testing the hebb function") {
  CHECK(weight_matrix[0][0] == 0.0f); //nessuna autoconnessione
  CHECK(weight_matrix[1][1] == weight_matrix[0][0]);  //nessuna autoconnessione
  CHECK(weight_matrix[0][1] == -0.5f);
  CHECK(weight_matrix[1][0] == weight_matrix[0][1]);// simmetria
  CHECK(weight_matrix[1][2] == 0.5f);
  CHECK(weight_matrix[2][1] == weight_matrix[1][2]); //simmetria
  }

 SUBCASE("testing the neuron_update function") {
        std::vector<int> x{1, -1, 1, -1};

        auto new_x0 = neuron_update(0, x, weight_matrix); //aggiorna lo stato del neurone 0 tenendo gli altri neuroni come input e calcolando la somma pesata tramite weight_matrix.
        CHECK(new_x0[0] == -1);
        CHECK(new_x0[1] == -1);
        CHECK(new_x0[2] == 1);
        CHECK(new_x0[3] == -1);

        auto new_x1 = neuron_update(1, x, weight_matrix);
        CHECK(new_x1[0] == 1);
        CHECK(new_x1[1] == 1);
        CHECK(new_x1[2] == 1);
        CHECK(new_x1[3] == -1);
}
}

TEST_CASE("testing energy_function") {

    SUBCASE("energy of a simple 2-neuron network") {
        std::vector<int> x{1, -1};
        std::vector<std::vector<float>> W{
            {0.0f, 1.0f},
            {1.0f, 0.0f}
        };

        // calcolo manuale: E = -0.5 * (0*1*1 + 1*1*-1 + 1*-1*1 + 0*-1*-1)
        // E = -0.5 * (-2) = 1
        float expected_energy = 1.0f;
        float E = energy_function(x, W);
        CHECK(std::fabs(E - expected_energy) < 1e-6); //la differenza fra il calcolo manuale e della funzione è circa 0
    } //fabs calcola il valore assoluto di un numero in virgola mobile (float o double).

    SUBCASE("energy of a 3-neuron network") {
        std::vector<int> x{1, -1, 1};
        std::vector<std::vector<float>> W{
            {0.0f, 0.5f, -0.5f},
            {0.5f, 0.0f, 1.0f},
            {-0.5f, 1.0f, 0.0f}
        };

        // calcolo manuale: 
        // sum = 0*1*1 + 0.5*1*-1 + -0.5*1*1 +
        //       0.5*-1*1 + 0* -1*-1 + 1*-1*1 +
        //       -0.5*1*1 + 1*1*-1 + 0*1*1 = -2.5
        // E = -0.5 * (-2.5) = 1.25
        float expected_energy = 2.0f;
        float E = energy_function(x, W);
        CHECK(std::fabs(E - expected_energy) < 1e-6);
    }

    SUBCASE("energy is zero for zero state vector") {
        std::vector<int> x{0, 0, 0};
        std::vector<std::vector<float>> W{
            {0.0f, 1.0f, -1.0f},
            {1.0f, 0.0f, 0.5f},
            {-1.0f, 0.5f, 0.0f}
        };

        float E = energy_function(x, W);
        CHECK(E == 0.0f);
    }

    SUBCASE("energy with symmetric weights") {
        std::vector<int> x{1, 1};
        std::vector<std::vector<float>> W{
            {0.0f, 2.0f},
            {2.0f, 0.0f}
        };
        // sum = 0*1*1 + 2*1*1 + 2*1*1 + 0*1*1 = 4
        // E = -0.5 * 4 = -2
        float E = energy_function(x, W);
        CHECK(std::fabs(E + 2.0f) < 1e-6);
    }
}



