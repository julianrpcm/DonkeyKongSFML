#include <filesystem>
#include <iostream>
#include "Level.hpp"
#include "Player.hpp"
#include "BasicEnemy.hpp"

std::string getProjectPath() {
    auto path = std::filesystem::current_path(); // Comienza en build/Debug
    while (!std::filesystem::exists(path / "CMakeLists.txt") && path.has_parent_path()) {
        path = path.parent_path();
    }
    return path.string();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Level Test");
    Level level;
    Player player;
    sf::Clock clock;

    std::string tmxPath = getProjectPath() + "/assets/maps/world.tmx";

    if (!level.loadFromFile(tmxPath))
        return 1;

    const auto& baseColliders = level.getCollisionRects();
    std::vector<sf::FloatRect> enemyGroundColliders;

    for (const auto& c : level.getCollisionRects()) {
        enemyGroundColliders.push_back(c);
    }
    for (const auto& c : level.getLadderBlockersRects()) {
        enemyGroundColliders.push_back(c);
    }

    std::vector<std::unique_ptr<Enemy>> enemies;
    

    if (!enemyGroundColliders.empty())
        enemies.push_back(std::make_unique<BasicEnemy>(enemyGroundColliders[0]));

    player.setLevel(&level);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime= clock.restart().asSeconds();

        const auto& ladders = level.getLadderRects();

        std::vector<sf::FloatRect> activeBlockers;
        if (level.getLadderBlockersEnabled()) {
            activeBlockers = level.getLadderBlockersRects();
        }

        player.update(deltaTime, baseColliders, ladders, activeBlockers, enemies);

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, enemyGroundColliders);
        }

        // Eliminar enemigos muertos
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy) {
                    return enemy->isDead();
                }),
            enemies.end()
        );

        window.clear();
        level.draw(window);
        player.draw(window);

        for (auto& enemy : enemies) {
            enemy->draw(window);
        }

        window.display();
    }

    return 0;
}
