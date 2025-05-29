#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class BarrelProjectile {
public:
    BarrelProjectile(const sf::Vector2f& startPosition,
        const std::vector<sf::FloatRect>& groundColliders);

    void update(float deltaTime,
        const std::vector<sf::FloatRect>& groundColliders,
        const std::vector<sf::FloatRect>& ladders);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    bool isOffScreen() const;

private:
    sf::RectangleShape shape;
    float velocityY = 0.f;
    float verticalSpeed = 60.f;
    float horizontalSpeed = 50.f;
    int direction = 0;
    bool onGround = false;
    const float gravity = 300.f;
};
