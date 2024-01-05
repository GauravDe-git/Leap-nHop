#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Platform.h"

class Game {
public:
    Game();

    void run();

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Music backgroundMusic;
    sf::Sound jumpSound;
    sf::SoundBuffer jumpBuffer;
    sf::Clock burstTimer;
    sf::Clock enemySpawnTimer;

    std::vector<Platform> platformShapes;
    std::vector<Enemy> enemies;

    Player character;

    sf::Text livesText;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;

    bool gameOver;
    bool bouncingLeft;
    bool bouncingRight;

    int lives;
    int score;
    float velocity;

    void handleEvents();
    void update();
    void render();
    void restartGame();
    void spawnEnemies();
    void checkCollisions();
};
