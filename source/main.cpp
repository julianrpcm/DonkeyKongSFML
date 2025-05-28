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

    BasicEnemy* enemyPtr = nullptr;
    

    if (!baseColliders.empty())
        enemyPtr = new BasicEnemy(enemyGroundColliders[0]);

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

        player.update(deltaTime, baseColliders, ladders, activeBlockers);
        enemyPtr->update(deltaTime, enemyGroundColliders);

        window.clear();
        level.draw(window);
        player.draw(window);
        enemyPtr->draw(window);
        window.display();
    }

    return 0;
}
