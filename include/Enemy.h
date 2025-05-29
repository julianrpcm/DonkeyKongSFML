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

protected:
    sf::RectangleShape shape;
    sf::Sprite sprite;
    int direction = -1;
    int health = 100;

    sf::FloatRect getFootCheck() const {
        const float footWidth = 6.f;
        const float footHeight = 2.f;
        const float offsetY = 2.f;

        float x = shape.getPosition().x + shape.getSize().x / 2.f - footWidth / 2.f;
        float y = shape.getPosition().y + shape.getSize().y + offsetY;

        return { x, y, footWidth, footHeight };
    }
};
