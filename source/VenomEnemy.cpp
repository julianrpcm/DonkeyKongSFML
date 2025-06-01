#include "VenomEnemy.h"
#include <iostream>

VenomEnemy::VenomEnemy(const sf::FloatRect& platformCollider, const std::string& rootPath)
    : assetPath(rootPath) {

    if (!idleTexture.loadFromFile(assetPath + "/assets/sprites/Venom/Idle.png")) {
        std::cerr << "Failed to load Idle.png\n";
    }
    if (!shootTexture.loadFromFile(assetPath + "/assets/sprites/Venom/Poison.png")) {
        std::cerr << "Failed to load Poison.png\n";
    }
    if (!deathTexture.loadFromFile(assetPath + "/assets/sprites/Venom/Dead.png")) {
        std::cerr << "Failed to load Dead.png\n";
    }

    frameRect = { 0, 0, 64, 64 };
    sprite.setTexture(idleTexture);
    sprite.setTextureRect(frameRect);
    sprite.setScale(1.f, 1.f);

    float spriteWidth = static_cast<float>(frameRect.width);
    float spriteHeight = static_cast<float>(frameRect.height);

    float xStart = platformCollider.left + (platformCollider.width - spriteWidth) / 2.f;
    float y = platformCollider.top - spriteHeight;
    sprite.setPosition({ xStart, y });

    float shapeWidth = 24.f;
    float shapeHeight = 24.f;
    float shapeX = platformCollider.left + (platformCollider.width - shapeWidth) / 2.f;
    float shapeY = platformCollider.top - shapeHeight;

    shape.setSize({ shapeWidth, shapeHeight });
    shape.setPosition({ shapeX, shapeY });

    setAnimation(AnimState::Idle);
}

void VenomEnemy::setAnimation(AnimState state) {
    currentState = state;
    frameTimer = 0.f;
    frameIndex = 0;

    switch (state) {
    case AnimState::Idle:
        sprite.setTexture(idleTexture);
        frameCount = 5;
        frameTime = 0.1f;
        break;
    case AnimState::Shoot:
        sprite.setTexture(shootTexture);
        frameCount = 7;
        frameTime = 0.08f;
        break;
    case AnimState::Death:
        sprite.setTexture(deathTexture);
        frameCount = 2;
        frameTime = 0.15f;
        break;
    }

    frameRect = { 0, 0, 64, 64 };
    sprite.setTextureRect(frameRect);
}

void VenomEnemy::update(float dt, const std::vector<sf::FloatRect>&) {
    if (isDead()) {
        if (isDying) {
            frameTimer += dt;
            if (frameTimer >= frameTime) {
                frameTimer = 0.f;
                frameIndex++;
                if (frameIndex >= frameCount) {
                    isDying = false;
                    return;
                }
                frameRect.left = frameIndex * 64;
                sprite.setTextureRect(frameRect);
            }
        }
        return;
    }

    frameTimer += dt;
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        frameIndex = (frameIndex + 1) % frameCount;
        frameRect.left = frameIndex * 64;
        sprite.setTextureRect(frameRect);
    }

    if (isShooting) {
        if (frameIndex == frameCount - 1) {
            isShooting = false;
            setAnimation(AnimState::Idle);

            sf::FloatRect venomBounds = shape.getGlobalBounds();
            sf::Vector2f spawnPos;
            spawnPos.x = venomBounds.left + (venomBounds.width / 2.f) - 32.f;
            spawnPos.y = venomBounds.top + venomBounds.height - 64.f;

            projectiles.emplace_back(std::make_unique<Venom>(spawnPos, assetPath));
        }
    }
    else {
        shootTimer += dt;
        if (shootTimer >= shootCooldown) {
            shootTimer = 0.f;
            isShooting = true;
            setAnimation(AnimState::Shoot);
        }
    }

    for (auto& venom : projectiles) {
        venom->update(dt);
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::unique_ptr<Venom>& v) { return v->isExpired(); }),
        projectiles.end()
    );
}

void VenomEnemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (const auto& v : projectiles) {
        v->draw(window);
    }
}

sf::FloatRect VenomEnemy::getBounds() const {
    return shape.getGlobalBounds();
}

const std::vector<std::unique_ptr<Venom>>& VenomEnemy::getProjectiles() const {
    return projectiles;
}

void VenomEnemy::updateHitboxPosition() {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    float width = 24.f, height = 24.f;
    float x = spriteBounds.left + (spriteBounds.width - width) / 2.f;
    float y = spriteBounds.top + spriteBounds.height - height;
    shape.setSize({ width, height });
    shape.setPosition({ x, y });
}

void VenomEnemy::takeDamage(int damage) {
    if (!isDying && health > 0) {
        health -= damage;
        if (health <= 0) {
            health = 0;
            isDying = true;
            setAnimation(AnimState::Death);
        }
    }
}

void VenomEnemy::setShootCooldown(float seconds)
{
    shootCooldown = seconds;
}
