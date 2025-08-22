#ifndef STROZ_HPP
#define STROZ_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
namespace hp{

bool isSpriteClicked(const sf::Sprite& sprite, sf::Vector2f mousePos);

int draw_stroz();
}  // namespace pf

#endif