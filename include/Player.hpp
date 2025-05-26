#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void handleInput();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    float speed;
};
