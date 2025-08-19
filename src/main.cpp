#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "graphics.hpp"
#include "image_processor.hpp"

int main() {
  ImageResized interp("resources/images/source", "resources/images/resized");
  interp.process();
  ImageNoised corrump("resources/images/resized", "resources/images/noised");
  corrump.process();
  ImageZoomed zoom("resources/images/resized", "resources/images/zoomed");
  zoom.process();
  ImageZoomed zoom_w_noise("resources/images/noised",
                          "resources/images/zoomed_w_noise");
  zoom_w_noise.process();
  
  draw();
  return 0;

}