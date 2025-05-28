#include "Enemy.hpp"
#include <iostream>

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
        std::cout << "Enemigo dañado, vida restante: " << health << "\n";
    }
}

bool Enemy::isDead() const {
    return health <= 0;
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}

void Enemy::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}
