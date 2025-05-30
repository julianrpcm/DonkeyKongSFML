#include "Enemy.h"
#include <iostream>

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::isDead() const {
    return health <= 0;
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}

void Enemy::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
    shape.setPosition(pos);
}

void Enemy::updateAnimation(float deltaTime)
{
    animationTimer += deltaTime;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        frameIndex = (frameIndex + 1) % frameCount;

        currentFrame.left = static_cast<int>(frameWidth) * frameIndex;
        currentFrame.top = 0;
        currentFrame.width = static_cast<int>(frameWidth);
        currentFrame.height = static_cast<int>(frameHeight);

        sprite.setTextureRect(currentFrame);
    }

    // Reflejar sprite según dirección
    sprite.setScale(direction > 0 ? visualScale : -visualScale, visualScale);
    sprite.setOrigin(direction > 0 ? 0.f : frameWidth, 0.f);
}
