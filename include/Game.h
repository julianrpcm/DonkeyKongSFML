#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <list>

#include "Level.h"
#include "Player.h"
#include "BasicEnemy.h"
#include "BossEnemy.h"
#include "ScoreManager.h"
#include "Coin.h"
#include "VenomEnemy.h"
#include <SpeedRune.h>

class Game {
public:
    Game();
    void run();

private:
    enum class EnemyType {
        Basic,
        Venom,
        Boss
    };

    enum class GameState {
        MainMenu,
        Playing,
        GameOver
    };

    GameState gameState = GameState::MainMenu;

    std::string getProjectPath();
    void processEvents();
    void update(float deltaTime);
    void render();
    void restartLevel();
    void restartLevelFromBeginning();
    void loadLevel();

    void initUI();
    void updateUI();
    void spawnEnemyAt(EnemyType type, int colliderIndex);
    void spawnRandomCoin();
    void spawnRandomRune();

    sf::RenderWindow window;
    sf::Clock clock;

    Level level;
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<BossEnemy> bossPtr;

    std::vector<sf::FloatRect> baseColliders;
    std::vector<sf::FloatRect> enemyGroundColliders;
    std::vector<sf::FloatRect> barrelGroundColliders;

    std::list<Coin> coins;
    std::vector<SpeedRune> runes;
    ScoreManager scoreManager;

    float enemySpeed = 100.f;
    float spawnDelay = 3.f;
    float spawnTimer = 0.f;
    int currentLevel = 1;

    //UI
    sf::Font font;

    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackground;
    sf::Text titleText;
    sf::Text playText;
    sf::Text exitText;

    sf::Text scoreText;
    sf::Text levelText;

    sf::RectangleShape deathOverlay;
    sf::Text retryText;
    sf::Text quitText;
    sf::Text gameOverText;
    bool showDeathMenu = false;

};
