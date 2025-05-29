#include "Venom.h"
#include <iostream>

Venom::Venom(const sf::Vector2f& position, const std::string& rootPath) {
    if (!texture.loadFromFile(rootPath + "/assets/sprites/Venom/Cloud_posion.png")) {
        std::cerr << "No se pudo cargar Cloud_posion.png\n";
    }

    frameRect = { 0, 0, 16, 16 }; // asumiendo que cada frame es 16x16
    sprite.setTexture(texture);
    sprite.setTextureRect(frameRect);
    sprite.setPosition(position);
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
}

void Venom::draw(sf::RenderWindow& window) const {
    if (!isExpired()) window.draw(sprite);
}

bool Venom::isExpired() const {
    return timer >= lifetime;
}

sf::FloatRect Venom::getBounds() const {
    return sprite.getGlobalBounds();
}
