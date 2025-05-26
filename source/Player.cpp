#include "Player.hpp"

Player::Player() {
    shape.setSize({ 50.f, 50.f });
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(100.f, 100.f);
    speed = 200.f;
}

void Player::handleInput() {
    // ahora no hace nada por sí sola
}

void Player::update(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        movement.x += speed * deltaTime;

    shape.move(movement);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
