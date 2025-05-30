#pragma once

#include <SFML/Graphics.hpp>

class Enemy {
public:
    virtual ~Enemy() = default;

    virtual void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;

    virtual void takeDamage(int damage);

    bool isDead() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);
    virtual void updateAnimation(float deltaTime);


protected:
    sf::RectangleShape shape;

    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect currentFrame;
    float animationTimer = 0.f;
    int frameIndex = 0;
    int frameCount = 1;
    float frameWidth = 32.f;
    float frameHeight = 32.f;
    float animationSpeed = 0.1f;
    float visualScale = 1.f;


    int direction = -1;
    int health = 100;

    sf::FloatRect getFootCheck() const {
        const float footWidth = 6.f;
        const float footHeight = 2.f;
        const float offsetY = 2.f;

        sf::FloatRect spriteBounds = sprite.getGlobalBounds();

        float x = sprite.getPosition().x + spriteBounds.width / 2.f - footWidth / 2.f;
        float y = sprite.getPosition().y + spriteBounds.height + offsetY;

        return { x, y, footWidth, footHeight };
    }
};
