#include "VenomEnemy.h"
#include <iostream>

VenomEnemy::VenomEnemy(const sf::Vector2f& position, const std::string& rootPath) : assetPath(rootPath) {
    if (!idleTexture.loadFromFile(assetPath + "/assets/sprites/Venom/Idle.png")) {
        std::cerr << "Error cargando Idle.png\n";
    }
    if (!shootTexture.loadFromFile(assetPath + "/assets/sprites/Venom/Poison.png")) {
        std::cerr << "Error cargando Poison.png\n";
    }

    sprite.setTexture(idleTexture);
    frameRect = { 0, 0, 16, 16 };
    sprite.setTextureRect(frameRect);
    sprite.setPosition(position);

    sprite.setScale(100.f, 100.f);
}

void VenomEnemy::update(float dt, const std::vector<sf::FloatRect>&) {
    shootTimer += dt;

    if (shootTimer >= shootCooldown) {
        shootTimer = 0.f;
        startShooting(assetPath);
    }

    frameTimer += dt;
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % totalFrames;
        frameRect.left = currentFrame * frameRect.width;
        sprite.setTextureRect(frameRect);
    }

    for (auto& venom : projectiles) {
        venom->update(dt);
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::unique_ptr<Venom>& v) { return v->isExpired(); }),
        projectiles.end()
    );
}

void VenomEnemy::startShooting(const std::string& rootPath) {
    sprite.setTexture(shootTexture);
    currentFrame = 0;
    frameRect = { 0, 0, 16, 16 };
    sprite.setTextureRect(frameRect);

    sf::Vector2f spawnPos = sprite.getPosition();
    spawnPos.x += sprite.getGlobalBounds().width / 2.f - 8.f;
    spawnPos.y -= 16.f;

    projectiles.emplace_back(std::make_unique<Venom>(spawnPos, rootPath));

    sprite.setTexture(idleTexture);
}

void VenomEnemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (const auto& v : projectiles) {
        v->draw(window);


    }
}

sf::FloatRect VenomEnemy::getBounds() const {
    return sprite.getGlobalBounds();
}

const std::vector<std::unique_ptr<Venom>>& VenomEnemy::getProjectiles() const {
    return projectiles;
}