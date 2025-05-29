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
    VenomEnemy(const sf::Vector2f& position, const std::string& rootPath);

    void update(float dt, const std::vector<sf::FloatRect>& groundColliders) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    const std::vector<std::unique_ptr<Venom>>& getProjectiles() const;

private:
    sf::Texture idleTexture;
    sf::Texture shootTexture;
    sf::Sprite sprite;
    sf::IntRect frameRect;

    float frameTimer = 0.f;
    float frameTime = 0.1f;
    int currentFrame = 0;
    int totalFrames = 5;

    float shootCooldown = 3.f;
    float shootTimer = 0.f;
    bool isShooting = false;

    std::vector<std::unique_ptr<Venom>> projectiles;

    void startShooting(const std::string& rootPath);
    std::string assetPath;
};