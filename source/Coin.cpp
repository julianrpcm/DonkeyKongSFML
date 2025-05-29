#include "Coin.h"

Coin::Coin(const sf::Vector2f& position) {
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
}

void Coin::draw(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(shape);
}

sf::FloatRect Coin::getBounds() const {
    return shape.getGlobalBounds();
}

bool Coin::isCollected() const {
    return collected;
}

void Coin::collect() {
    collected = true;
}
