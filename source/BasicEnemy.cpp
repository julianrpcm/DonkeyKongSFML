#include "BasicEnemy.h"
#include <iostream>

BasicEnemy::BasicEnemy(const sf::FloatRect& platformCollider, const std::string& projectPath) {

    health = 1;

    visualScale = 1.2f;

    frameCount = 8;
    frameWidth = 64.f;
    frameHeight = 64.f;
    animationSpeed = 0.1f;

    if (!texture.loadFromFile(projectPath + "/assets/sprites/Enemy/Walk.png")) {
        std::cerr << "Error loading Walk.png\n";
    }


    if (!deadTexture.loadFromFile(projectPath + "/assets/sprites/Enemy/Dead.png")) {
        std::cerr << "Failed to load death texture\n";
    }

    sprite.setTexture(texture);
    currentFrame = { 0, 0, static_cast<int>(frameWidth), static_cast<int>(frameHeight) };
    sprite.setTextureRect(currentFrame);
    sprite.setScale(visualScale, visualScale);

    float spriteWidth = frameWidth * visualScale;
    float spriteHeight = frameHeight * visualScale;

    float xStart = platformCollider.left + (platformCollider.width / 2.f) - (spriteWidth / 2.f);
    float y = platformCollider.top - spriteHeight;
    sprite.setPosition({ xStart, y });

    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    float marginX = 4.f;
    float marginY = 6.f;

    shape.setSize({
        spriteBounds.width - 2.f * marginX,
        spriteBounds.height - 2.f * marginY
        });
    shape.setPosition({
        spriteBounds.left + marginX,
        spriteBounds.top + marginY
        });

    leftEdge = platformCollider.left;
    rightEdge = platformCollider.left + platformCollider.width;
}

void BasicEnemy::update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) {
    sf::FloatRect footCheck = getFootCheck();
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

    if (isDead()) {
        if (isDying) {
            updateAnimation(deltaTime);
            if (frameIndex >= frameCount - 1 && animationTimer == 0.f) {
                isDying = false;
            }
        }
        return;
    }

    float dx = direction * speed * deltaTime;
    sprite.move(dx, 0.f);
    updateHitboxPosition();

    updateAnimation(deltaTime);
}

void BasicEnemy::draw(sf::RenderWindow& window) {
    if (isDead() && !isDying) return;
    window.draw(sprite);
}

sf::FloatRect BasicEnemy::getBounds() const {
    return shape.getGlobalBounds();
}

void BasicEnemy::setSpeed(float s)
{
    speed = s;
}

void BasicEnemy::updateHitboxPosition() {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    float desiredWidth = spriteBounds.width - 35.f;
    float desiredHeight = spriteBounds.height - 30.f;

    shape.setSize({ desiredWidth, desiredHeight });

    shape.setPosition({
        spriteBounds.left + (spriteBounds.width - desiredWidth) / 2.f,
        spriteBounds.top + (spriteBounds.height - desiredHeight)
        });
}

void BasicEnemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0 && !isDying) {
        health = 0;
        isDying = true;
        frameIndex = 0;
        frameCount = 3;
        animationSpeed = 0.15f;

        sprite.setTexture(deadTexture);
    }
}


