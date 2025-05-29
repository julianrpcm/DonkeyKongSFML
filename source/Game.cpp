#include "Game.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

Game::Game()
    : window(sf::VideoMode(1280, 960), "Level Test") {
    std::string tmxPath = getProjectPath() + "/assets/maps/world.tmx";

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

    player.setLevel(&level);

    if (!enemyGroundColliders.empty())
        enemies.push_back(std::make_unique<BasicEnemy>(enemyGroundColliders[0]));

    bossPtr = std::make_unique<BossEnemy>(enemyGroundColliders[0]);
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

    // Eliminar enemigos muertos
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return enemy->isDead();
            }),
        enemies.end()
    );

    // Reiniciar nivel si jefe muere
    if (bossPtr->isDead()) {
        restartLevel();
    }
}

void Game::render() {
    window.clear();
    level.draw(window);
    player.draw(window);
    bossPtr->draw(window);

    for (auto& enemy : enemies) {
        enemy->draw(window);
    }

    window.display();
}

void Game::restartLevel() {
    currentLevel++;
    enemySpeed *= 1.2f;
    spawnDelay *= 0.85f;

    enemies.clear();
    bossPtr = std::make_unique<BossEnemy>(enemyGroundColliders[0]);
    // Aquí puedes volver a añadir enemigos, monedas, etc.
}
