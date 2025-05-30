#include "BossEnemy.h"
#include <iostream>

BossEnemy::BossEnemy(const sf::FloatRect& platformCollider, const std::string& projectPath) 
    : projectPath(projectPath)
{
    health = 100;

    shape.setSize({ 24.f, 24.f });
    shape.setFillColor(sf::Color::Magenta);

    float xStart = platformCollider.left + (platformCollider.width / 2.f) - (shape.getSize().x / 2.f);
    float y = platformCollider.top - shape.getSize().y;
    shape.setPosition({ xStart, y });

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

    }

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
    if (!isDead()) window.draw(shape);

    for (auto& p : projectiles) {
        p->draw(window);
    }
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
