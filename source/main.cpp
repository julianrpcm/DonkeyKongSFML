#include <filesystem>
#include <iostream>
#include "Level.hpp"
#include "Player.hpp"

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
    player.setLevel(&level);
    sf::Clock clock;

    std::string tmxPath = getProjectPath() + "/assets/maps/world.tmx";

    if (!level.loadFromFile(tmxPath))
        return 1;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        const auto& baseColliders = level.getCollisionRects();
        const auto& ladders = level.getLadderRects();

        std::vector<sf::FloatRect> activeBlockers;
        if (level.getLadderBlockersEnabled()) {
            activeBlockers = level.getLadderBlockersRects(); // se pasan solo si están activos
        }

        player.update(dt, baseColliders, ladders, activeBlockers);


        window.clear();
        level.draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}
