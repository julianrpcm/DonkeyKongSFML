#pragma once

#include "Enemy.h"
#include "BarrelProjectile.h"
#include <vector>
#include <memory>

class BossEnemy : public Enemy {
public:
    explicit BossEnemy(const sf::FloatRect& platformCollider, const std::string& projectPath);

    void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void takeDamage(int damage) override;
    void updateAnimation(float deltaTime) override;

    void setLadders(const std::vector<sf::FloatRect>& ladders);
    void setGroundColliders(const std::vector<sf::FloatRect>& ground);
    std::vector<std::unique_ptr<BarrelProjectile>>& getProjectiles();

    enum class BossState { Walking, Throwing, Dead };
    BossState state = BossState::Walking;

private:
    void launchBarrel();
    void updateHitboxPosition();

    float speed = 40.f;
    float cooldown = 3.f;
    float timer = 0.f;

    float throwPauseDuration = 0.5f;
    float throwPauseTimer = 0.f;

    float leftEdge;
    float rightEdge;
    float spriteYOffset = 30.f;

    std::vector<sf::FloatRect> currentLadders;
    std::vector<sf::FloatRect> currentGround;
    std::vector<std::unique_ptr<BarrelProjectile>> projectiles;

    std::string projectPath;
    sf::Texture deadTexture;
};
