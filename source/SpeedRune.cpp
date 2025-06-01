#include "SpeedRune.h"
#include <iostream>

SpeedRune::SpeedRune(const sf::Vector2f& pos, const std::string& rootPath) {
    if (!texture.loadFromFile(rootPath + "/assets/sprites/Rune.png")) {
        std::cerr << "Failed to load rune texture\n";
    }
    frameRect = { 0, 0, 16, 16 };
    sprite.setTexture(texture);
    sprite.setTextureRect(frameRect);
    sprite.setPosition(pos);
    sprite.setScale(2.f, 2.f);
}

void SpeedRune::update(float dt) {
    if (collected) return;

    frameTimer += dt;
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        frameIndex = (frameIndex + 1) % frameCount;
        frameRect.left = frameIndex * 16;
        sprite.setTextureRect(frameRect);
    }
}

void SpeedRune::draw(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(sprite);
}

bool SpeedRune::isCollected() const {
    return collected;
}

void SpeedRune::collect() {
    collected = true;
}

sf::FloatRect SpeedRune::getBounds() const {
    return sprite.getGlobalBounds();
}