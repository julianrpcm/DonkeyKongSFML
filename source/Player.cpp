#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include <iostream>

Player::Player() {
    shape.setSize({ 24.f, 24.f });
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(100.f, 100.f);
}

void Player::update(float dt,
    const std::vector<sf::FloatRect>& colliders,
    const std::vector<sf::FloatRect>& ladders,
    const std::vector<sf::FloatRect>& laddersBlockers,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    BossEnemy* boss) {
    if (!alive) return;

    // Movimiento horizontal
    velocity.x = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  velocity.x = -150.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) velocity.x = 150.f;

    // Detección de escaleras
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    bool isTouchingLadder = false;
    for (const auto& ladder : ladders) {
        if (playerBounds.intersects(ladder)) {
            isTouchingLadder = true;
            break;
        }
    }

    // Activar/desactivar modo escalera
    if (!onLadder && isTouchingLadder && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
        onLadder = true;
        if (levelRef) levelRef->setLadderBlockersEnabled(false);
    }
    if (onLadder && !isTouchingLadder) {
        onLadder = false;
        if (levelRef) levelRef->setLadderBlockersEnabled(true);
    }

    // Movimiento vertical
    if (onLadder) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))      velocity.y = -100.f;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) velocity.y = 100.f;
        else                                                     velocity.y = 0.f;
    }
    else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && onGround)
            velocity.y = -300.f;

        velocity.y += gravity * dt;
        onGround = false;
    }

    // Aplicar colisiones
    std::vector<sf::FloatRect> allColliders = colliders;
    allColliders.insert(allColliders.end(), laddersBlockers.begin(), laddersBlockers.end());

    shape.move(velocity.x * dt, 0.f);
    handleCollision(allColliders);
    shape.move(0.f, velocity.y * dt);
    if (!onLadder) handleCollision(allColliders);

    // Interacciones
    handleEnemyCollisions(enemies);
    handleBossProjectiles(boss);
    handleBossCollision(boss);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

void Player::setPosition(const sf::Vector2f& pos)
{
    shape.setPosition(pos);
}

void Player::handleCollision(const std::vector<sf::FloatRect>& colliders) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    for (const auto& rect : colliders) {
        if (playerBounds.intersects(rect)) {
            sf::FloatRect intersection;
            playerBounds.intersects(rect, intersection);

            if (intersection.width < intersection.height) {
                if (playerBounds.left < rect.left) shape.move(-intersection.width, 0.f);
                else                              shape.move(intersection.width, 0.f);
                velocity.x = 0.f;
            }
            else {
                if (playerBounds.top < rect.top) {
                    shape.move(0.f, -intersection.height);
                    onGround = true;
                }
                else {
                    shape.move(0.f, intersection.height);
                }
                velocity.y = 0.f;
            }
            playerBounds = shape.getGlobalBounds();
        }
    }
}

void Player::handleEnemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isDead()) continue;
        tryStompEnemy(enemy->getBounds(), [&]() {
            enemy->takeDamage(100);
            });
    }
}

void Player::handleBossCollision(BossEnemy* boss) {
    if (!boss || boss->isDead()) return;
    tryStompEnemy(boss->getBounds(), [&]() {
        boss->takeDamage(100);
        });
}

void Player::handleBossProjectiles(BossEnemy* boss) {
    if (!boss) return;

    for (const auto& barrel : boss->getProjectiles()) {
        if (barrel->getBounds().intersects(getBounds())) {
            die();
        }
    }
}

bool Player::tryStompEnemy(const sf::FloatRect& enemyBounds, std::function<void()> onStomp) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    if (playerBounds.intersects(enemyBounds)) {
        float playerBottom = playerBounds.top + playerBounds.height;
        float enemyTop = enemyBounds.top;

        if (playerBottom <= enemyTop + 5.f && velocity.y > 0) {
            onStomp();
            velocity.y = -200.f;
            return true;
        }
        else {
            die();
            return false;
        }
    }
    return false;
}

bool Player::isAlive() const {
    return alive;
}

void Player::die() {
    if (!alive) return;
    alive = false;
    std::cout << "El jugador ha muerto\n";
}
