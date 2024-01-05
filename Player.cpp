#include "Player.h"

Player::Player(float size, float x, float y) {
    shape.setSize(sf::Vector2f(size, size));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(size / 2.0f, size / 2.0f);
}

void Player::move(float x, float y) {
    shape.move(x, y);
}

void Player::rotate(float angle) {
    shape.rotate(angle);
}

const sf::RectangleShape& Player::getShape() const {
    return shape;
}
