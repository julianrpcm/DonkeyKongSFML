#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Venom {
public:
    Venom(const sf::Vector2f& position, const std::string& rootPath);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    bool isExpired() const;
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect frameRect;
    sf::RectangleShape shape;

    float timer = 0.f;
    const float lifetime = 1.f;

    float frameTime = 0.08f;
    float frameTimer = 0.f;
    int currentFrame = 0;
    const int totalFrames = 10;
};
