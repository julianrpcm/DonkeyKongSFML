#pragma once
#include "Enemy.h"

class BasicEnemy : public Enemy {
public:
    explicit BasicEnemy(const sf::FloatRect& platformCollider);

    void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void setSpeed(float s);

private:
    float speed = 50.f;
    float leftEdge;
    float rightEdge;
};
