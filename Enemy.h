#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float radius, float x, float y);

    void moveDown(float speed);
    const sf::CircleShape& getShape() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};
