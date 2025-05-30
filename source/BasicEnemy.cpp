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

    sprite.setTexture(texture);
    currentFrame = { 0, 0, static_cast<int>(frameWidth), static_cast<int>(frameHeight) };
    sprite.setTextureRect(currentFrame);
    sprite.setScale(visualScale, visualScale);

    // Posicionar sprite (sin depender del shape)
    float spriteWidth = frameWidth * visualScale;
    float spriteHeight = frameHeight * visualScale;

    float xStart = platformCollider.left + (platformCollider.width / 2.f) - (spriteWidth / 2.f);
    float y = platformCollider.top - spriteHeight;
    sprite.setPosition({ xStart, y });

    // HITBOX ajustada al sprite visible
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    float marginX = 4.f; // puedes ajustar
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

    float dx = direction * speed * deltaTime;
    sprite.move(dx, 0.f);
    //shape.setPosition(sprite.getPosition());
    updateHitboxPosition();

    updateAnimation(deltaTime);
}

void BasicEnemy::draw(sf::RenderWindow& window) {
    if (isDead()) return;
    window.draw(sprite);

    // DEBUG: dibuja la hitbox en rojo transparente
   /* shape.setFillColor(sf::Color::Transparent);
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

sf::FloatRect BasicEnemy::getBounds() const {
    return shape.getGlobalBounds();
}

void BasicEnemy::setSpeed(float s)
{
    speed = s;
}

void BasicEnemy::updateHitboxPosition() {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    float desiredWidth = spriteBounds.width - 35.f;  // más estrecho (ajusta aquí)
    float desiredHeight = spriteBounds.height - 30.f; // opcional: o déjalo igual

    shape.setSize({ desiredWidth, desiredHeight });

    // Centrar horizontalmente y alinear con los pies del sprite
    shape.setPosition({
        spriteBounds.left + (spriteBounds.width - desiredWidth) / 2.f,
        spriteBounds.top + (spriteBounds.height - desiredHeight)
        });
}

