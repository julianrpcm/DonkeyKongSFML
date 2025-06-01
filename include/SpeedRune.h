#pragma once
#include <SFML/Graphics.hpp>

class SpeedRune {
public:
    SpeedRune(const sf::Vector2f& pos, const std::string& rootPath);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    bool isCollected() const;
    void collect();
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool collected = false;

    sf::IntRect frameRect;
    int frameIndex = 0;
    float frameTimer = 0.f;
    const float frameTime = 0.15f;
    const int frameCount = 4;
};