#pragma once
#include "Enemy.hpp"

class BasicEnemy : public Enemy {
public:
    BasicEnemy(const sf::FloatRect& platformCollider);

    void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() const override;


private:
    float speed = 50.0f;
    int direction = -1; // -1 left, 1 = right

    float leftEdge;
    float rightEdge;

    sf::RectangleShape shape;
};
