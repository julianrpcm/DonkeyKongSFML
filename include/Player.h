#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Enemy.h"

class Level;
class Enemy;
class BossEnemy;

class Player {
public:
    Player();

    void update(float dt,
        const std::vector<sf::FloatRect>& colliders,
        const std::vector<sf::FloatRect>& ladders,
        const std::vector<sf::FloatRect>& laddersBlockers,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        BossEnemy* boss);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    void setLevel(Level* lvl) { levelRef = lvl; }

    bool isAlive() const;
    void die();

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float gravity = 500.f;
    bool onGround = false;
    bool onLadder = false;
    bool touchingLadder = false;
    bool alive = true;

    Level* levelRef = nullptr;

    void handleCollision(const std::vector<sf::FloatRect>& colliders);
    void handleEnemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies);
    void handleBossCollision(BossEnemy* boss);
    void handleBossProjectiles(BossEnemy* boss);
    bool tryStompEnemy(const sf::FloatRect& enemyBounds, std::function<void()> onStomp);
};
