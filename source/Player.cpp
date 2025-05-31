#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include <iostream>

Player::Player() {

    velocity = { 0.f, 0.f };
    alive = true;
    onGround = false;
    onLadder = false;

    hitbox.setSize({ 24.f, 30.f });
    hitbox.setOrigin(12.f, 15.f); 
    hitbox.setPosition(100.f, 100.f);

    sprite.setOrigin(32.f, 32.f);
    sprite.setTextureRect(sf::IntRect(0, WALK_ROW * frameHeight, frameWidth, frameHeight));

    sprite.setPosition(hitbox.getPosition());

    // Estado inicial
    state = PlayerState::Idle;

}

void Player::update(float dt,
    const std::vector<sf::FloatRect>& colliders,
    const std::vector<sf::FloatRect>& ladders,
    const std::vector<sf::FloatRect>& laddersBlockers,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    BossEnemy* boss) {

    if (!alive) {
        updateAnimation(dt); 
        return; 
    }

    // Movimiento horizontal
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
        velocity.x = -150.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        velocity.x = 150.f;

    if (velocity.x != 0) 
        state = PlayerState::Walk;
    else                 
        state = PlayerState::Idle;

    // Detección de escaleras
    sf::FloatRect playerBounds = hitbox.getGlobalBounds();
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
        if (levelRef) 
            levelRef->setLadderBlockersEnabled(false);
    }
    if (onLadder && !isTouchingLadder) {
        onLadder = false;
        if (levelRef) 
            levelRef->setLadderBlockersEnabled(true);
    }

    // Movimiento vertical
    if (onLadder) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y = -100.f;
            state = PlayerState::Climb;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y = 100.f;
            state = PlayerState::Climb;
        }
        else {
            velocity.y = 0.f;
            state = PlayerState::Climb;
        }
    }
    else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && onGround) {
            velocity.y = -300.f;
            state = PlayerState::Jump;
        }

        velocity.y += gravity * dt;
        onGround = false;
    }

    // Aplicar colisiones
    std::vector<sf::FloatRect> allColliders = colliders;
    allColliders.insert(allColliders.end(), laddersBlockers.begin(), laddersBlockers.end());

    hitbox.move(velocity.x * dt, 0.f);
    handleCollision(allColliders);
    hitbox.move(0.f, velocity.y * dt);

    if (!onLadder) 
        handleCollision(allColliders);

    if (onGround && state == PlayerState::Jump) {
        if (velocity.x == 0.f)
            state = PlayerState::Idle;
        else
            state = PlayerState::Walk;
    }

    sprite.setPosition(hitbox.getPosition());
    updateAnimation(dt);

    // Interacciones
    handleEnemyCollisions(enemies);
    handleBossProjectiles(boss);
    handleBossCollision(boss);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite);

    // DEBUG: Cuadro para visualizar los límites del sprite
   /* sf::RectangleShape box(sf::Vector2f(64.f, 64.f));
    box.setPosition(sprite.getPosition().x - 32.f, sprite.getPosition().y); // compensar origen
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Blue);
    box.setOutlineThickness(1.f);
    window.draw(box);

    // Dibujar la hitbox lógica (en rojo semitransparente)
    sf::RectangleShape debugBox(hitbox);
    debugBox.setFillColor(sf::Color(255, 0, 0, 100));  // rojo con transparencia
    debugBox.setOutlineColor(sf::Color::Red);
    debugBox.setOutlineThickness(1.f);
    window.draw(debugBox);*/
}

sf::FloatRect Player::getBounds() const {
    return hitbox.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f& pos)
{
    hitbox.setPosition(pos);
    sprite.setPosition(hitbox.getPosition());
}

void Player::handleCollision(const std::vector<sf::FloatRect>& colliders) {
    sf::FloatRect playerBounds = hitbox.getGlobalBounds();

    for (const auto& rect : colliders) {
        if (playerBounds.intersects(rect)) {
            sf::FloatRect intersection;
            playerBounds.intersects(rect, intersection);

            if (intersection.width < intersection.height) {
                if (playerBounds.left < rect.left) 
                    hitbox.move(-intersection.width, 0.f);
                else                               
                    hitbox.move(intersection.width, 0.f);
                velocity.x = 0.f;
            }
            else {
                if (playerBounds.top < rect.top) {
                    hitbox.move(0.f, -intersection.height);
                    onGround = true;
                }
                else {
                    hitbox.move(0.f, intersection.height);
                }
                velocity.y = 0.f;
            }
            playerBounds = hitbox.getGlobalBounds();
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
    sf::FloatRect playerBounds = hitbox.getGlobalBounds();

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
    if (!alive) 
        return;
    alive = false;
    state = PlayerState::Dead;
    currentFrame = 0;
    animationTimer = 0.f;
}

void Player::loadTexture(const std::string& projectPath)
{
    if (!texture.loadFromFile(projectPath + "/assets/sprites/Player/Player.png")) {
        std::cerr << "Error cargando la textura del jugador\n";
    }
    sprite.setTexture(texture);
}

bool Player::hasDeathAnimationFinished() const
{
    return state == PlayerState::Dead && currentFrame >= DEAD_MAX_FRAMES - 1;
}

void Player::reset(const sf::Vector2f& startPos)
{
    alive = true;
    state = PlayerState::Idle;
    currentFrame = 0;
    animationTimer = 0.f;
    velocity = { 0.f, 0.f };
    onGround = false;
    onLadder = false;

    hitbox.setPosition(startPos);
    sprite.setPosition(hitbox.getPosition());
}

void Player::updateAnimation(float dt)
{
    int row = 0;
    int maxFrames = 1;

    switch (state) {
    case PlayerState::Idle:
        row = IDLE_ROW; 
        maxFrames = 3;      
        break;
    case PlayerState::Walk:
        row = WALK_ROW;    
        maxFrames = 5;     
        break;
    case PlayerState::Jump:
        row = JUMP_ROW;
        maxFrames = 5;
        break;
    case PlayerState::Climb:
        row = CLIMB_ROW;
        maxFrames = 7;
        break;
    case PlayerState::Dead:
        row = DEAD_ROW;
        maxFrames = DEAD_MAX_FRAMES;
        break;
    }

    animationTimer += dt;
    if (animationTimer >= animationSpeed) {
        animationTimer = 0.f;

        if (state == PlayerState::Dead) { 
            if (currentFrame < maxFrames - 1)
                currentFrame++;
        }
        else {
            currentFrame = (currentFrame + 1) % maxFrames;
        }
    }


    sprite.setTextureRect(sf::IntRect(
        currentFrame * frameWidth,
        row * frameHeight,
        frameWidth,
        frameHeight
    ));

    // Voltear sprite si va a la izquierda
    if (velocity.x < 0)
        sprite.setScale(-1.f, 1.f); // flip
    else if (velocity.x > 0)
        sprite.setScale(1.f, 1.f);  // normal
}