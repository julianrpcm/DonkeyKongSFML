#include "BossEnemy.h"
#include <iostream>

BossEnemy::BossEnemy(const sf::FloatRect& platformCollider, const std::string& projectPath) 
    : projectPath(projectPath)
{
    health = 100;

    // --- Parámetros visuales y de animación ---
    visualScale = 1.5f;
    frameCount = 8;
    frameWidth = 64.f;
    frameHeight = 64.f;
    animationSpeed = 0.1f;

    if (!texture.loadFromFile(projectPath + "/assets/sprites/Boss/Move.png")) {
        std::cerr << "Failed to load boss texture\n";
    }

    sprite.setTexture(texture);
    currentFrame = { 0, 0, static_cast<int>(frameWidth), static_cast<int>(frameHeight) };
    sprite.setTextureRect(currentFrame);
    sprite.setScale(visualScale, visualScale);

    // --- Posicionar sprite centrado sobre la plataforma ---
    float spriteWidth = frameWidth * visualScale;
    float spriteHeight = frameHeight * visualScale;

    float xStart = platformCollider.left + (platformCollider.width / 2.f) - (spriteWidth / 2.f);
    float y = platformCollider.top - (frameHeight * visualScale) + 8.f;

    sprite.setPosition({ xStart, y });

    // --- HITBOX (shape) ajustada al sprite ---
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

    // --- Límite de movimiento ---
    leftEdge = platformCollider.left;
    rightEdge = platformCollider.left + platformCollider.width;

    direction = -1;

}

void BossEnemy::update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) {
    timer += deltaTime;

    if (throwPauseTimer > 0.f) {
        throwPauseTimer -= deltaTime;
        if (throwPauseTimer < 0.f)
            throwPauseTimer = 0.f;
    }
    else {
        if (timer >= cooldown) {
            timer = 0.f;
            launchBarrel();
            throwPauseTimer = throwPauseDuration;
        }

        float dx = direction * speed * deltaTime;
        shape.move(dx, 0.f);

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

        // Reposicionar sprite en función del nuevo shape
        float spriteX = shape.getPosition().x + (shape.getSize().x / 2.f) - (frameWidth * visualScale / 2.f);
        float spriteY = shape.getPosition().y + shape.getSize().y - (frameHeight * visualScale);
        sprite.setPosition(spriteX, spriteY);

        updateHitboxPosition();
    }

    updateAnimation(deltaTime);

    for (auto& p : projectiles) {
        p->update(deltaTime, groundColliders, currentLadders);
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::unique_ptr<BarrelProjectile>& b) {
                return b->isOffScreen();
            }),
        projectiles.end()
    );
}

void BossEnemy::draw(sf::RenderWindow& window) {
    if (!isDead()) window.draw(sprite);

    for (auto& p : projectiles) {
        p->draw(window);
    }

    // DEBUG: dibuja la hitbox en rojo transparente
 shape.setFillColor(sf::Color::Transparent);
 shape.setOutlineColor(sf::Color::Red);
 shape.setOutlineThickness(1.f);
 window.draw(shape);

 sf::RectangleShape spriteBox({ frameWidth * visualScale, frameHeight * visualScale });
 spriteBox.setPosition(sprite.getPosition());
 spriteBox.setFillColor(sf::Color::Transparent);
 spriteBox.setOutlineColor(sf::Color::Blue);
 spriteBox.setOutlineThickness(1.f);
 window.draw(spriteBox);
}

sf::FloatRect BossEnemy::getBounds() const {
    return shape.getGlobalBounds();
}

void BossEnemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void BossEnemy::setLadders(const std::vector<sf::FloatRect>& ladders) {
    currentLadders = ladders;
}

void BossEnemy::setGroundColliders(const std::vector<sf::FloatRect>& ground) {
    currentGround = ground;
}

std::vector<std::unique_ptr<BarrelProjectile>>& BossEnemy::getProjectiles() {
    return projectiles;
}

void BossEnemy::launchBarrel() {
    sf::Vector2f pos = shape.getPosition();
    pos.x += (shape.getSize().x / 2.f) - 8.f;
    pos.y += shape.getSize().y - 1.f;

    projectiles.push_back(std::make_unique<BarrelProjectile>(pos, currentGround, projectPath));
}

void BossEnemy::updateHitboxPosition() {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    float desiredWidth = spriteBounds.width - 35.f;
    float desiredHeight = spriteBounds.height - 30.f;

    shape.setSize({ desiredWidth, desiredHeight });

    shape.setPosition({
        spriteBounds.left + (spriteBounds.width - desiredWidth) / 2.f,
        spriteBounds.top + (spriteBounds.height - desiredHeight)
        });
}

