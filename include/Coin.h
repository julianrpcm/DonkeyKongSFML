#pragma once
#include <SFML/Graphics.hpp>

class Coin {
public:
    Coin(const sf::Vector2f& position);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isCollected() const;
    void collect();

private:
    sf::CircleShape shape;
    bool collected = false;
};
