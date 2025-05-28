#include "BasicEnemy.hpp"
#include <iostream>

BasicEnemy::BasicEnemy(const sf::FloatRect& platformCollider)
{
    health = 1;

    shape.setSize({ 24.f, 24.f });
    shape.setFillColor(sf::Color::Red);

    float xStart = platformCollider.left + (platformCollider.width / 2.f) - (shape.getSize().x / 2.f);
    float y = platformCollider.top - shape.getSize().y;

    shape.setPosition({ xStart, y });

    leftEdge = platformCollider.left;
    rightEdge = platformCollider.left + platformCollider.width;
}

void BasicEnemy::update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) {
    
    sf::FloatRect footCheck;
    if (direction == -1) {
        footCheck = {
            shape.getPosition().x - 1.f,
            shape.getPosition().y + shape.getSize().y,
            2.f, 2.f
        };
    }
    else {
        footCheck = {
            shape.getPosition().x + shape.getSize().x + 1.f,
            shape.getPosition().y + shape.getSize().y,
            2.f, 2.f
        };
    }

    bool groundAhead = false;

    for (const auto& collider : groundColliders) {
        if (footCheck.intersects(collider)) {
            groundAhead = true;
            break;
        }
    }

    if (!groundAhead) {
        direction *= -1;
    }


    // Movimiento
    float dx = direction * speed * deltaTime;
    shape.move(dx, 0.f);

}

void BasicEnemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
