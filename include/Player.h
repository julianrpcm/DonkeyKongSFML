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

    void loadTexture(const std::string& projectPath);
    bool hasDeathAnimationFinished() const;


    enum class PlayerState {
        Idle,
        Walk,
        Jump,
        Climb,
        Dead,
    };

private:
    sf::RectangleShape hitbox;

    sf::Texture texture;
    sf::Sprite sprite;
    PlayerState state;


    int currentFrame = 0;
    float animationTimer = 0.f;
    float animationSpeed = 0.1f;

    const int frameWidth = 64;
    const int frameHeight = 64;

    const int IDLE_ROW = 0;
    const int WALK_ROW = 3;
    const int JUMP_ROW = 17;
    const int CLIMB_ROW = 10;
    const int DEAD_ROW = 8;
    const int DEAD_MAX_FRAMES = 12;



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
    void updateAnimation(float dt);
};
