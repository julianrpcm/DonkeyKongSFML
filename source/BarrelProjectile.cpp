#include "BarrelProjectile.h"
#include <cstdlib> // rand
#include <ctime>
#include <iostream>

BarrelProjectile::BarrelProjectile(const sf::Vector2f& startPosition, const std::vector<sf::FloatRect>& groundColliders) {
    shape.setSize({ 16.f, 16.f });
    shape.setFillColor(sf::Color::Yellow);

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
        pos.y = closestY - shape.getSize().y;
    }

    shape.setPosition(pos);
    std::srand(static_cast<unsigned>(std::time(nullptr))); 
    direction = (std::rand() % 2 == 0) ? -1 : 1;
    velocityY = 0.f;
}

void BarrelProjectile::update(float deltaTime,
    const std::vector<sf::FloatRect>& groundColliders,
    const std::vector<sf::FloatRect>&) {

    float dx = direction * horizontalSpeed * deltaTime;
    float dy = velocityY * deltaTime;

    // Verificar colisión horizontal
    sf::FloatRect boundsX = shape.getGlobalBounds();
    boundsX.left += dx;
    bool collidesX = false;
    for (const auto& rect : groundColliders) {
        if (boundsX.intersects(rect)) {
            collidesX = true;
            break;
        }
    }
    if (collidesX) {
        direction *= -1;
    }
    else {
        shape.move(dx, 0.f);
    }

    // Aplicar gravedad y verificar colisión vertical
    velocityY += gravity * deltaTime;
    sf::FloatRect boundsY = shape.getGlobalBounds();
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
        shape.move(0.f, velocityY * deltaTime);
    }

}

void BarrelProjectile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect BarrelProjectile::getBounds() const {
    return shape.getGlobalBounds();
}

bool BarrelProjectile::isOffScreen() const {
    sf::Vector2f pos = shape.getPosition();
    return pos.y > 1000.f || pos.x < -50.f || pos.x > 1500.f;
}
