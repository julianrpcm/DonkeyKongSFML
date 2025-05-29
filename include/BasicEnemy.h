#pragma once
#include "Enemy.h"

class BasicEnemy : public Enemy {
public:
    BasicEnemy(const sf::FloatRect& platformCollider);

    void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() const override;


private:
    float speed = 50.0f;

    float leftEdge;
    float rightEdge;

};
