#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class BarrelProjectile {
public:
    BarrelProjectile(const sf::Vector2f& startPosition, const std::vector<sf::FloatRect>& groundColliders);

    void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders, const std::vector<sf::FloatRect>& ladders);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    bool isOffScreen() const;

private:
    sf::RectangleShape shape;
    float verticalSpeed = 60.f;
    float horizontalSpeed = 50.f;
    int direction = 0; // 0: cayendo, -1: izquierda, 1: derecha
    bool onGround = false;
    float velocityY = 0.f;
    const float gravity = 300.f;
};
