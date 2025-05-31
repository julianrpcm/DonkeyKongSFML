#include "Game.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

Game::Game()
    : window(sf::VideoMode(1280, 1024), "Level Test") {

    std::string rootPath = getProjectPath();
    std::string tmxPath = rootPath + "/assets/maps/world.tmx";

    if (!level.loadFromFile(tmxPath)) {
        std::cerr << "Error loading level\n";
        exit(1);
    }

    baseColliders = level.getCollisionRects();
    barrelGroundColliders = baseColliders;
    enemyGroundColliders = level.getCollisionRects();

    for (const auto& c : level.getLadderBlockersRects()) {
        enemyGroundColliders.push_back(c);
    }

    player.loadTexture(getProjectPath());
    player.setLevel(&level);

    if (!enemyGroundColliders.empty())
        enemies.push_back(std::make_unique<BasicEnemy>(enemyGroundColliders[14], getProjectPath()));

    bossPtr = std::make_unique<BossEnemy>(enemyGroundColliders[0], getProjectPath());
    
    sf::FloatRect base = enemyGroundColliders[35];
    sf::Vector2f position = {
        base.left + base.width / 2.f - 8.f,
        base.top - 16.f
    };

    venomEnemy = std::make_unique<VenomEnemy>(position, getProjectPath());


    initUI();
}

std::string Game::getProjectPath() {
    auto path = std::filesystem::current_path();
    while (!std::filesystem::exists(path / "CMakeLists.txt") && path.has_parent_path()) {
        path = path.parent_path();
    }
    return path.string();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(float deltaTime) {
    auto ladders = level.getLadderRects();

    std::vector<sf::FloatRect> activeBlockers;
    if (level.getLadderBlockersEnabled()) {
        activeBlockers = level.getLadderBlockersRects();
    }

    player.update(deltaTime, baseColliders, ladders, activeBlockers, enemies, bossPtr.get());

    for (auto& enemy : enemies) {
        enemy->update(deltaTime, enemyGroundColliders);
    }

    bossPtr->setLadders(ladders);
    bossPtr->setGroundColliders(barrelGroundColliders);
    bossPtr->update(deltaTime, enemyGroundColliders);

    if (venomEnemy) {
        venomEnemy->update(deltaTime, enemyGroundColliders);

        for (const auto& venom : venomEnemy->getProjectiles()) {
            if (!venom->isExpired() && venom->getBounds().intersects(player.getBounds())) {
                player.die();
            }
        }
    }

    //Score
    for (const auto& enemy : enemies) {
        if (enemy->shouldGiveScore()) {
            scoreManager.addPoints(100);
            enemy->hasGivenScore = true;
        }
    }
    // Eliminar enemigos muertos
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return enemy->canBeRemoved();
            }),
        enemies.end()
    );

    //Recoger monedas
    for (auto& coin : coins) {
        coin.update(deltaTime);

        if (!coin.isCollected() && coin.getBounds().intersects(player.getBounds())) {
            coin.collect();
            scoreManager.addPoints(50);
        }
    }


    if (bossPtr && bossPtr->shouldGiveScore()) {
        scoreManager.addPoints(500);
        bossPtr->hasGivenScore = true;
    }

    // Reiniciar nivel si jefe muere
    if (bossPtr && bossPtr->canBeRemoved()) {
        restartLevel();
    }

    updateUI();
}

void Game::render() {

    window.clear();
    level.draw(window);
    player.draw(window);
    bossPtr->draw(window);
    venomEnemy->draw(window);
    window.draw(levelText);
    window.draw(scoreText);
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }

    for (const auto& coin : coins) {
        coin.draw(window);
    }


    window.display();
}

void Game::restartLevel() {
    std::string projectPath = getProjectPath();
    currentLevel++;
    enemySpeed *= 1.2f;
    spawnDelay *= 0.85f;

    enemies.clear();
    bossPtr = std::make_unique<BossEnemy>(enemyGroundColliders[0], getProjectPath());
    coins.clear();

    {
        auto enemy = std::make_unique<BasicEnemy>(enemyGroundColliders[14], getProjectPath());
        enemy->setSpeed(enemySpeed);
        enemies.push_back(std::move(enemy));
    }
    {
        auto enemy = std::make_unique<BasicEnemy>(enemyGroundColliders[35], getProjectPath());
        enemy->setSpeed(enemySpeed);
        enemies.push_back(std::move(enemy));
    }
    {
        auto enemy = std::make_unique<BasicEnemy>(enemyGroundColliders[21], getProjectPath());
        enemy->setSpeed(enemySpeed);
        enemies.push_back(std::move(enemy));
    }

    sf::FloatRect base = enemyGroundColliders[35];
    sf::Vector2f position = {
        base.left + base.width / 2.f - 8.f,
        base.top - 16.f
    };

    venomEnemy = std::make_unique<VenomEnemy>(position, getProjectPath());

    coins.emplace_back(sf::Vector2f(400.f, 300.f), projectPath);
    coins.emplace_back(sf::Vector2f(600.f, 200.f), projectPath);
    coins.emplace_back(sf::Vector2f(800.f, 400.f), projectPath);

    player.setPosition({
        enemyGroundColliders[32].left,
        enemyGroundColliders[32].top - player.getBounds().height
        });
}

void Game::initUI()
{
    if (!font.loadFromFile(getProjectPath() + "/assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "No se pudo cargar la fuente\n";
        return;
    }

    levelText.setFont(font);
    levelText.setCharacterSize(28);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(20.f, 20.f);
    levelText.setString("Level: 1");

    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20.f, 60.f);
    scoreText.setString("Score: 0");
}

void Game::updateUI()
{
    scoreText.setString("Score: " + std::to_string(scoreManager.getScore()));
    levelText.setString("Level: " + std::to_string(currentLevel));
}
