#include "BossEnemy.h"
#include "Enemy.h"
#include <iostream>

BossEnemy::BossEnemy(const sf::FloatRect& platformCollider, const std::string& projectPath) 
    : projectPath(projectPath)
{
    health = 100;

    // --- Parámetros visuales y de animación ---
    visualScale = 2.f;
    frameCount = 8;
    frameWidth = 64.f;
    frameHeight = 64.f;
    animationSpeed = 0.1f;

    rowIndex = 1;

    if (!texture.loadFromFile(projectPath + "/assets/sprites/Boss/Fire golem.png")) {
        std::cerr << "Failed to load boss texture\n";
    }

    sprite.setTexture(texture);

    currentFrame = {
    0,
    static_cast<int>(frameHeight) * rowIndex, // Segunda fila
    static_cast<int>(frameWidth),
    static_cast<int>(frameHeight)
    };

    sprite.setTextureRect(currentFrame);
    sprite.setScale(visualScale, visualScale);

    // --- Posicionar sprite centrado sobre la plataforma ---
    float spriteWidth = frameWidth * visualScale;
    float spriteHeight = frameHeight * visualScale;

    float xStart = platformCollider.left + (platformCollider.width / 2.f) - (spriteWidth / 2.f);
    float y = (platformCollider.top - spriteHeight) + spriteYOffset;
    
    sprite.setPosition({ xStart, y });

    // --- HITBOX (shape) ajustada al sprite ---
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
     float marginX = 10.f;
    float marginY = 12.f;

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

    if (state == BossState::Dead) {
        updateAnimation(deltaTime);
        if (frameIndex == frameCount - 1 && animationTimer == 0.f) {
            isDying = false;
        }
        return;
    }

    timer += deltaTime;

    if (throwPauseTimer > 0.f) {
        throwPauseTimer -= deltaTime;
        if (throwPauseTimer < 0.f) {
            throwPauseTimer = 0.f;
            state = BossState::Walking;
            frameIndex = 0;
        }

    }
    else {
        if (timer >= cooldown) {
            timer = 0.f;
            launchBarrel();
            throwPauseTimer = throwPauseDuration;
            state = BossState::Throwing;
            frameIndex = 0;
        }

        float dx = direction * speed * deltaTime;
        sprite .move(dx, 0.f);
        updateHitboxPosition();

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
    }

    if (state == BossState::Walking) {
        rowIndex = 1;
        frameCount = 8;
    }
    else if (state == BossState::Throwing) {
        rowIndex = 5;
        frameCount = 2;
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

    if (state != BossState::Dead || isDying) 
        window.draw(sprite);

    for (auto& p : projectiles) {
        p->draw(window);
    }

   /* // DEBUG: dibuja la hitbox en rojo transparente
 shape.setFillColor(sf::Color::Transparent);
 shape.setOutlineColor(sf::Color::Red);
 shape.setOutlineThickness(1.f);
 window.draw(shape);

 sf::RectangleShape spriteBox({ frameWidth * visualScale, frameHeight * visualScale });
 spriteBox.setPosition(sprite.getPosition());
 spriteBox.setFillColor(sf::Color::Transparent);
 spriteBox.setOutlineColor(sf::Color::Blue);
 spriteBox.setOutlineThickness(1.f);
 window.draw(spriteBox);*/
}

sf::FloatRect BossEnemy::getBounds() const {
    return shape.getGlobalBounds();
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

    float desiredWidth = spriteBounds.width * 0.4f;
    float desiredHeight = spriteBounds.height * 0.6f;

    shape.setSize({ desiredWidth, desiredHeight });

    shape.setPosition({
        spriteBounds.left + (spriteBounds.width - desiredWidth) / 2.f,
        spriteBounds.top + (spriteBounds.height - desiredHeight) / 2.f
        });
}

void BossEnemy::updateAnimation(float deltaTime)
{
    animationTimer += deltaTime;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;
        frameIndex = (frameIndex + 1) % frameCount;

        currentFrame.left = static_cast<int>(frameWidth) * frameIndex;
        currentFrame.top = static_cast<int>(frameHeight) * rowIndex; // mantiene la fila
        currentFrame.width = static_cast<int>(frameWidth);
        currentFrame.height = static_cast<int>(frameHeight);

        sprite.setTextureRect(currentFrame);
    }

    // Reflejar según dirección
    sprite.setScale(direction > 0 ? -visualScale : visualScale, visualScale);
    sprite.setOrigin(direction > 0 ? frameWidth : 0.f, 0.f);
}

void BossEnemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0 && !isDying) {
        health = 0;
        isDying = true;
        state = BossState::Dead;

        rowIndex = 7;
        frameCount = 6;
        frameIndex = 0;
        animationSpeed = 0.15f;

    }
}

