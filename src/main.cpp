#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <exception>

#include "graphics.hpp"
#include "hebb.hpp"
#include "image_processor.hpp"
#include "recall.hpp"

int main() {
  try {
    hp::ImageResized interp("resources/images/source",
                            "resources/images/resized");
    interp.process();
    hp::ImageNoised corrump("resources/images/resized",
                            "resources/images/noised");
    corrump.process();
    hp::ImageZoomed zoom("resources/images/resized", "resources/images/zoomed");
    zoom.process();
    hp::ImageZoomed zoom_w_noise("resources/images/noised",
                                 "resources/images/zoomed_w_noise");
    zoom_w_noise.process();
    hp::draw();
    // crea matrice di hebb
    hp::Hebb create_matrix("resources/images/resized", "data");
    create_matrix.process();
  }
  catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}