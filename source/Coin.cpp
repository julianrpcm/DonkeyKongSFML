#include "Coin.h"
#include "Game.h"
#include <iostream>

Coin::Coin(const sf::Vector2f& position, const std::string& rootPath) {

    std::string path = rootPath + "/assets/sprites/Coin.png";

    if (!texture.loadFromFile(path)) {
        std::cerr << "No se pudo cargar la textura de moneda\n";
    }

    frameRect = { 0, 0, 10, 16 }; // ancho y alto por frame
    sprite.setTexture(texture);
    sprite.setTextureRect(frameRect);
    sprite.setPosition(position);
    sprite.setScale(3.f, 3.f);
}

void Coin::update(float dt) {
    if (collected) return;

    frameTimer += dt;

    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % totalFrames;

        frameRect.left = currentFrame * frameRect.width;
        sprite.setTextureRect(frameRect);
    }
}

void Coin::draw(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(sprite);
}

sf::FloatRect Coin::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Coin::isCollected() const {
    return collected;
}

void Coin::collect() {
    collected = true;
}
