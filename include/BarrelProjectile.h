#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class BarrelProjectile {
public:
    BarrelProjectile(const sf::Vector2f& startPosition,
        const std::vector<sf::FloatRect>& groundColliders,
        const std::string& projectPath);

    void update(float deltaTime,
        const std::vector<sf::FloatRect>& groundColliders,
        const std::vector<sf::FloatRect>& ladders);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    bool isOffScreen() const;
    void updateHitboxPosition();

private:
    sf::RectangleShape hitbox;
    sf::Sprite sprite;
    std::vector<sf::Texture> textures;
    std::string projectPath;

    float animationTimer = 0.f;
    int currentFrame = 0;
    const float frameDuration = 0.08f;

    float velocityY = 0.f;
    float verticalSpeed = 60.f;
    float horizontalSpeed = 50.f;
    int direction = 0;
    bool onGround = false;
    const float gravity = 300.f;
};
