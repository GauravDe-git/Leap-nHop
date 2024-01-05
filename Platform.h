#pragma once
#include <SFML/Graphics.hpp>

class Platform {
public:
    Platform(float width, float height, float x, float y);

    const sf::RectangleShape& getShape() const;

private:
    sf::RectangleShape shape;
};

