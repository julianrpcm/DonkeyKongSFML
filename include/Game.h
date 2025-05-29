#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>

#include "Level.h"
#include "Player.h"
#include "BasicEnemy.h"
#include "BossEnemy.h"
#include "ScoreManager.h"

class Game {
public:
    Game();
    void run();

    sf::Font font;
    sf::Text scoreText;
    ScoreManager scoreManager;

private:
    std::string getProjectPath();
    void processEvents();
    void update(float deltaTime);
    void render();
    void restartLevel();
    void initUI();
    void updateUI();


    sf::RenderWindow window;
    sf::Clock clock;

    Level level;
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<BossEnemy> bossPtr;

    std::vector<sf::FloatRect> baseColliders;
    std::vector<sf::FloatRect> enemyGroundColliders;
    std::vector<sf::FloatRect> barrelGroundColliders;

    float enemySpeed = 100.f;
    float spawnDelay = 3.f;
    float spawnTimer = 0.f;

    int currentLevel = 1;
};
