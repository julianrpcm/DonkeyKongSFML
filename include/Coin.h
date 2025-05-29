#pragma once
#include <SFML/Graphics.hpp>

class Coin {
public:
    Coin(const sf::Vector2f& position, const std::string& rootPath);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    bool isCollected() const;
    void collect();

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect frameRect;

    float frameTime = 0.15f;
    float frameTimer = 0.f;
    int currentFrame = 0;
    const int totalFrames = 4;

    bool collected = false;
};
