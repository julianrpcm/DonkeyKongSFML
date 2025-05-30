#include "BarrelProjectile.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

BarrelProjectile::BarrelProjectile(const sf::Vector2f& startPosition,
    const std::vector<sf::FloatRect>& groundColliders,
    const std::string& projectPath) {

    for (int i = 1; i <= 8; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile(projectPath + "/assets/sprites/Barrel/" + std::to_string(i) + ".png")) {
            std::cerr << "Failed to load barrel frame: " << i << "\n";
        }
        textures.push_back(texture);
    }

    sprite.setTexture(textures[0]);
    sprite.setScale(visualScale, -visualScale);
    sprite.setOrigin(0.f, 0.f);

    sf::Vector2f pos = startPosition;
    float closestY = 10000.f;

    for (const auto& rect : groundColliders) {
        if (pos.x + 8.f >= rect.left && pos.x + 8.f <= rect.left + rect.width) {
            if (rect.top < closestY) {
                closestY = rect.top;
            }
        }
    }

    if (closestY != 10000.f) {
        pos.y = closestY - textures[0].getSize().y;
    }

    sprite.setPosition(pos);
    updateHitboxPosition();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    direction = (std::rand() % 2 == 0) ? -1 : 1;
    velocityY = 0.f;
}

void BarrelProjectile::update(float deltaTime,
    const std::vector<sf::FloatRect>& groundColliders,
    const std::vector<sf::FloatRect>&) {
    float dx = direction * horizontalSpeed * deltaTime;
    float dy = velocityY * deltaTime;

    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % textures.size();
        sprite.setTexture(textures[currentFrame]);
    }

    // Movimiento horizontal
    sf::FloatRect boundsX = hitbox.getGlobalBounds();
    boundsX.left += dx;

    sf::FloatRect boundsYOnly = hitbox.getGlobalBounds();
    boundsYOnly.top += velocityY * deltaTime;

    bool collidesX = false;

    for (const auto& rect : groundColliders) {
        if (boundsX.intersects(rect) && !boundsYOnly.intersects(rect)) {
            collidesX = true;
            break;
        }
    }

    if (collidesX) {
        direction *= -1;
    }
    else {
        sprite.move(dx, 0.f);
        updateHitboxPosition();
    }

    // Movimiento vertical con gravedad
    velocityY += gravity * deltaTime;
    sf::FloatRect boundsY = hitbox.getGlobalBounds();

    boundsY.top += velocityY * deltaTime;

    bool collidesY = false;

    for (const auto& rect : groundColliders) {
        if (boundsY.intersects(rect)) {
            collidesY = true;
            break;
        }
    }

    if (collidesY) {
        velocityY = 0.f;
    }
    else {
        sprite.move(0.f, velocityY * deltaTime);
        updateHitboxPosition();
    }

    sprite.setScale(direction > 0 ? visualScale : -visualScale, -visualScale);
    //updateHitboxPosition();
}

void BarrelProjectile::draw(sf::RenderWindow& window){
    window.draw(sprite);

    // DEBUG: dibujar hitbox
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    window.draw(hitbox);

    sf::FloatRect local = sprite.getLocalBounds();

    sf::RectangleShape spriteBox;
    spriteBox.setSize({ local.width, local.height });
    spriteBox.setOrigin(sprite.getOrigin());
    spriteBox.setPosition(sprite.getPosition());
    spriteBox.setScale(sprite.getScale()); // aplica reflejo
    spriteBox.setFillColor(sf::Color::Transparent);
    spriteBox.setOutlineColor(sf::Color::Blue); // Azul = sprite lógico
    spriteBox.setOutlineThickness(1.f);
    window.draw(spriteBox);

}

sf::FloatRect BarrelProjectile::getBounds() const {
    return hitbox.getGlobalBounds();
}

bool BarrelProjectile::isOffScreen() const {
    sf::Vector2f pos = sprite.getPosition();
    return pos.y > 1000.f || pos.x < -50.f || pos.x > 1500.f;
}

void BarrelProjectile::updateHitboxPosition() {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    float desiredWidth = spriteBounds.width - 35.f;
    float desiredHeight = spriteBounds.height - 30.f;

    hitbox.setSize({ desiredWidth, desiredHeight });

    hitbox.setPosition({
        spriteBounds.left + (spriteBounds.width - desiredWidth) / 2.f,
        spriteBounds.top + (spriteBounds.height - desiredHeight)
        });
}
