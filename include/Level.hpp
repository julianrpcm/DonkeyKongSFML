#pragma once
#include <tmxlite/Map.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Level {
public:
    bool loadFromFile(const std::string& tmxPath);

    void draw(sf::RenderWindow& window) const;

    const std::vector<sf::FloatRect>& getCollisionRects() const;
    const std::vector<sf::FloatRect>& getLadderRects() const;
    const std::vector<sf::FloatRect>& getLadderBlockersRects() const;

    void setLadderBlockersEnabled(bool enabled);
    bool getLadderBlockersEnabled() const;


private:
    tmx::Map map;

    std::vector<sf::FloatRect> collisionRects;
    std::vector<sf::FloatRect> ladderRects;
    std::vector<sf::FloatRect> ladderBlockers;

    bool ladderBlockersEnabled = true;


};
