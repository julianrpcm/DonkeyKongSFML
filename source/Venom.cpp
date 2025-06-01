#include "Venom.h"
#include <iostream>

Venom::Venom(const sf::Vector2f& position, const std::string& rootPath) {
    if (!texture.loadFromFile(rootPath + "/assets/sprites/Venom/Cloud_posion.png")) {
        std::cerr << "Failed to load Cloud_posion.png\n";
    }

    frameRect = { 0, 0, 64, 64 };
    sprite.setTexture(texture);
    sprite.setTextureRect(frameRect);
    sprite.setPosition(position);

    float shapeWidth = 40.f;
    float shapeHeight = 36.f;

    shape.setSize({ shapeWidth, shapeHeight });

    shape.setPosition({
        position.x + (frameRect.width - shapeWidth) / 2.f,
        position.y + frameRect.height - shapeHeight
        });

}

void Venom::update(float dt) {
    timer += dt;
    if (isExpired()) return;

    frameTimer += dt;
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % totalFrames;
        frameRect.left = currentFrame * frameRect.width;
        sprite.setTextureRect(frameRect);
    }

    sf::Vector2f pos = sprite.getPosition();
    shape.setPosition({
        pos.x + (frameRect.width - shape.getSize().x) / 2.f,
        pos.y + frameRect.height - shape.getSize().y
        });

}

void Venom::draw(sf::RenderWindow& window) const {
    if (!isExpired()) 
        window.draw(sprite);
}

bool Venom::isExpired() const {
    return timer >= lifetime;
}

sf::FloatRect Venom::getBounds() const {
    return shape.getGlobalBounds();
}
