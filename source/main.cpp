#include <SFML/Graphics.hpp>
#include "Player.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Donkey Kong SFML");
    Player player;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        player.handleInput();
        player.update(deltaTime);

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}
