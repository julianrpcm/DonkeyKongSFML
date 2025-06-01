#pragma once

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Venom.h"
#include <vector>
#include <memory>
#include <string>

class VenomEnemy : public Enemy {
public:
    VenomEnemy() = default;
    VenomEnemy(const sf::FloatRect& platformCollider, const std::string& rootPath);

    void update(float dt, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    const std::vector<std::unique_ptr<Venom>>& getProjectiles() const;

    void updateHitboxPosition();
    void takeDamage(int damage) override;
    void setShootCooldown(float seconds);


private:
    enum class AnimState { Idle, Shoot, Death };
    void setAnimation(AnimState state);

    sf::Texture idleTexture;
    sf::Texture shootTexture;
    sf::Texture deathTexture;

    sf::Sprite sprite;
    sf::RectangleShape shape;
    sf::IntRect frameRect;

    int frameIndex = 0;
    int frameCount = 1;
    float frameTime = 0.1f;
    float frameTimer = 0.f;

    AnimState currentState = AnimState::Idle;

    float shootTimer = 0.f;
    float shootCooldown = 10.f;
    bool isShooting = false;
    float shootAnimTimer = 0.f;
    float shootAnimDuration = 0.56f; 
    int shootFrameCount = 7;

    std::vector<std::unique_ptr<Venom>> projectiles;

    std::string assetPath;
};
