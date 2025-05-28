#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Level;

class Player {
public:
    Player();

    void update(float dt, const std::vector<sf::FloatRect>& colliders, const std::vector<sf::FloatRect>& ladders, const std::vector<sf::FloatRect>& laddersBlockers);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    void setLevel(Level* lvl) { levelRef = lvl; }


private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool onGround = false;
    bool onLadder = false;
    float gravity = 500.f;
    bool touchingLadder = false;

    class Level* levelRef = nullptr;


    void handleCollision(const std::vector<sf::FloatRect>& colliders);

};
