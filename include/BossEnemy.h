#pragma once

#include "Enemy.h"
#include "BarrelProjectile.h"

class BossEnemy : public Enemy {
public:
    BossEnemy(const sf::FloatRect& platformCollider);

    void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void takeDamage(int damage) override;
    void setLadders(const std::vector<sf::FloatRect>& ladders);
    void setGroundColliders(const std::vector<sf::FloatRect>& ground);

    std::vector<std::unique_ptr<BarrelProjectile>>& getProjectiles();

private:

    float speed = 40.f;

    float leftEdge;
    float rightEdge;

    float cooldown = 3.f;
    float timer = 0.f;
    bool isThrowing = false;
    std::vector<sf::FloatRect> currentLadders;
    std::vector<sf::FloatRect> currentGround; // nuevo

    float throwPauseDuration = 0.5f; // pausa al lanzar
    float throwPauseTimer = 0.f;




    std::vector<std::unique_ptr<BarrelProjectile>> projectiles;

    void launchBarrel();
};
