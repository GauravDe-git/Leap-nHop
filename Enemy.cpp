#include "Enemy.h"

Enemy::Enemy(float radius, float x, float y) {
    shape.setRadius(radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);
    velocity = sf::Vector2f(0.0f, 5.0f);
}

void Enemy::moveDown(float speed) {
    shape.move(velocity);
}

const sf::CircleShape& Enemy::getShape() const {
    return shape;
}
