#include "Platform.h"

Platform::Platform(float width, float height, float x, float y) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White);
}

const sf::RectangleShape& Platform::getShape() const {
    return shape;
}
