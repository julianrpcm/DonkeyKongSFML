#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    virtual ~Enemy() = default;

    virtual void update(float deltaTime, const std::vector<sf::FloatRect>& groundColliders) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void takeDamage(int damage);
    bool isDead() const;

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    virtual sf::FloatRect getBounds() const = 0;

protected:
    int health = 100;
    const float speed = 100.f;

    sf::Sprite sprite;
};
