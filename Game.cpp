// Game.cpp
#include "Game.h"
#include <iostream>
#include <random>

Game::Game()
    : window(sf::VideoMode(1280, 720), "Leap'nHop", sf::Style::Default),
    character(50.0f, (window.getSize().x - 50.0f) / 2.0f, 30.0f),
    gameOver(false),
    bouncingLeft(false),
    bouncingRight(false),
    lives(3),
    score(0),
    velocity(-13.0f) {
    window.setFramerateLimit(60);

    if (!font.loadFromFile("Assets/RetroGaming.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        std::exit(1);
    }

    if (!backgroundMusic.openFromFile("Assets/TheRush.ogg")) {
        std::cout << "Failed to load music!" << std::endl;
        std::exit(1);
    }

    if (!jumpBuffer.loadFromFile("Assets/jump.wav")) {
        std::cout << "Failed to load jump sound effect!" << std::endl;
        std::exit(1);
    }

    jumpSound.setBuffer(jumpBuffer);

    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    backgroundMusic.setVolume(70.f);

    // Set up initial platforms
    const int numShapes = 8;
    const float shapeWidth = window.getSize().x / numShapes;
    const float shapeHeight = 100.0f;

    for (int i = 0; i < numShapes; ++i) {
        platformShapes.emplace_back(shapeWidth, shapeHeight, i * shapeWidth, window.getSize().y - shapeHeight - 30.0f);
    }

    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(10.0f, 10.0f);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x / 2.0f - scoreText.getLocalBounds().width / 2.0f, 10.0f);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over");
    gameOverText.setPosition(window.getSize().x / 2.0f - gameOverText.getLocalBounds().width / 2.0f,
        window.getSize().y / 2.0f - gameOverText.getLocalBounds().height / 2.0f);

    restartText.setFont(font);
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press R to Restart");
    restartText.setPosition(window.getSize().x / 2.0f - restartText.getLocalBounds().width / 2.0f,
        gameOverText.getPosition().y + gameOverText.getLocalBounds().height + 20.0f);
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && gameOver) {
            // Restart the game
            restartGame();
        }
    }
}

void Game::update() {
    if (!gameOver && !bouncingRight && !bouncingLeft) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && character.getShape().getPosition().x > 0.0f) {
            character.move(-10.0f, 0.0f);
            character.rotate(-5.0f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && character.getShape().getPosition().x + character.getShape().getSize().x < window.getSize().x) {
            character.move(10.0f, 0.0f);
            character.rotate(5.0f);
        }
    }

    character.move(0.0f, velocity);
    velocity += 0.55f;

    bool onPlatform = false;

    for (auto it = platformShapes.begin(); it != platformShapes.end();) {
        if (character.getShape().getGlobalBounds().intersects(it->getShape().getGlobalBounds())) {
            jumpSound.play();
            score += std::rand() % 16 + 5; // Increase score by a random number
            velocity = -13.0f;
            onPlatform = true;

            // Remove the collided platform
            it = platformShapes.erase(it);
        }
        else {
            ++it;
        }
    }


    if (!onPlatform && character.getShape().getPosition().y > window.getSize().y) {
        character.move(0.0f, 10.0f);
    }

    if (character.getShape().getPosition().x <= 0.0f || character.getShape().getPosition().x + character.getShape().getSize().x >= window.getSize().x) {
        velocity = -13.0f;
        bouncingLeft = character.getShape().getPosition().x + character.getShape().getSize().x >= window.getSize().x;
        bouncingRight = character.getShape().getPosition().x <= 0.0f;
        burstTimer.restart();
    }

    if (bouncingRight || bouncingLeft) {
        float burstDuration = burstTimer.getElapsedTime().asSeconds();
        character.move((bouncingRight ? 5.0f : -5.0f), 0.0f);

        if (burstDuration >= 0.8f)
            bouncingLeft = bouncingRight = false;
    }

    if (platformShapes.size() < 5) {
        for (size_t i = 0; i < 3; i++) {
            platformShapes.emplace_back(window.getSize().x / 8.0f, 100.0f, std::rand() % static_cast<int>(window.getSize().x), window.getSize().y - 100.0f - 30.0f);
        }
    }
    // Move enemies downwards and remove enemies that go off the screen
    //for (auto& enemy : enemies) {
    //    enemy.moveDown(5.0f); // Adjust the speed as needed
    //}
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->moveDown(5.0f); // Adjust the speed as needed

        if (it->getShape().getPosition().y > window.getSize().y) {
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
    spawnEnemies();
    checkCollisions();
}

void Game::render() {
    window.clear();

    for (const auto& enemy : enemies) {
        window.draw(enemy.getShape());
    }

    for (const auto& platform : platformShapes) {
        window.draw(platform.getShape());
    }

    window.draw(character.getShape());

    livesText.setString("Lives: " + std::to_string(lives));
    scoreText.setString("Score: " + std::to_string(score));

    window.draw(livesText);
    window.draw(scoreText);

    if (gameOver) {
        window.draw(gameOverText);
        window.draw(restartText);
    }

    window.display();
}

void Game::restartGame() {
    gameOver = false;
    lives = 3;
    score = 0;
    velocity = -13.0f;

    // Set player position directly
    character.setPosition((window.getSize().x - character.getShape().getSize().x) / 2.0f, platformShapes[0].getShape().getPosition().y - character.getShape().getSize().y);

    // Clear platforms and recreate initial platforms
    platformShapes.clear();
    for (int i = 0; i < 8; ++i) {
        platformShapes.emplace_back(window.getSize().x / 8.0f, 100.0f, i * window.getSize().x / 8.0f, window.getSize().y - 100.0f - 30.0f);
    }

    // Clear enemies
    enemies.clear();

    // Restart enemy spawn timer
    enemySpawnTimer.restart();
}


void Game::spawnEnemies() {
    if (enemySpawnTimer.getElapsedTime().asSeconds() >= 2.0f && lives > 0) {
        enemies.emplace_back(10.0f, std::rand() % static_cast<int>(window.getSize().x), -10.0f);
        enemySpawnTimer.restart();
    }
}

void Game::checkCollisions() {
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (character.getShape().getGlobalBounds().intersects(it->getShape().getGlobalBounds())) {
            lives--;

            if (lives <= 0) {
                gameOver = true;
                break;
            }

            it = enemies.erase(it);
            continue;
        }

        it++;
    }

    if (character.getShape().getPosition().y > window.getSize().y && lives > 0) {
        lives--;

        if (lives > 0) {
            character.move((window.getSize().x - character.getShape().getSize().x) / 2.0f, 40.f);
            velocity = -13.0f;
        }
        else {
            gameOver = true;
        }
    }

    if (lives <= 0) {
        lives = 0;
    }

    if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        restartGame();
    }
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}
