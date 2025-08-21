#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
namespace hp{

bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos);

int draw();
}  // namespace pf

#endif