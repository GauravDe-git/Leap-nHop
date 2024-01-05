#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(float size, float x, float y);

    void move(float x, float y);
    void rotate(float angle);
    const sf::RectangleShape& getShape() const;
    void setPosition(float x, float y) {
        shape.setPosition(x, y);
    }

private:
    sf::RectangleShape shape;
};
