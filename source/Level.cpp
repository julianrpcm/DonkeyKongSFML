#include "Level.hpp"
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <iostream>

bool Level::loadFromFile(const std::string& tmxPath) {
    if (!map.load(tmxPath)) {
        std::cerr << "Failed to load map: " << tmxPath << "\n";
        return false;
    }

    auto tileSize = map.getTileSize();
    auto mapSize = map.getTileCount();

    for (const auto& layer : map.getLayers()) {
        if (!layer->getVisible()) continue;

        // CAPA DE COLISIONES
        if (layer->getName() == "Collisions" && layer->getType() == tmx::Layer::Type::Object) {
            const auto& objGroup = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objGroup.getObjects()) {
                auto pos = obj.getPosition();
                auto size = obj.getAABB();
                sf::FloatRect rect(pos.x, pos.y, size.width, size.height);
                collisionRects.push_back(rect);
            }
        }

        // CAPA DE ESCALERAS (OBJETOS)
        if (layer->getName() == "Ladders" && layer->getType() == tmx::Layer::Type::Object) {
            const auto& objGroup = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objGroup.getObjects()) {
                auto pos = obj.getPosition();
                auto size = obj.getAABB();
                sf::FloatRect rect(pos.x, pos.y, size.width, size.height);
                ladderRects.push_back(rect);
            }
        }

        if (layer->getName() == "LaddersBlockers" && layer->getType() == tmx::Layer::Type::Object) {
            const auto& objGroup = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objGroup.getObjects()) {
                auto pos = obj.getPosition();
                auto size = obj.getAABB();
                sf::FloatRect rect(pos.x, pos.y, size.width, size.height);
                ladderBlockers.push_back(rect);
            }
        }
        std::cout << "Capa detectada: " << layer->getName() << std::endl;
        std::cout << "Se cargaron " << collisionRects.size() << " colisiones.\n";
    }

    std::cout << "Mapa cargado: " << tmxPath << "\n";
    std::cout << "Colliders: " << collisionRects.size() << "\n";
    std::cout << "Ladders: " << ladderRects.size() << "\n";

    return true;
}

void Level::draw(sf::RenderWindow& window) const {
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(1.f);

    // Dibujar zonas de colisión (rojo)
    rect.setFillColor(sf::Color(255, 0, 0, 100)); // rojo semi-transparente
    for (const auto& box : collisionRects) {
        rect.setPosition(box.left, box.top);
        rect.setSize({ box.width, box.height });
        window.draw(rect);
    }

    // Dibujar zonas de escaleras (verde)
    rect.setFillColor(sf::Color(0, 255, 0, 100)); // verde semi-transparente
    for (const auto& ladder : ladderRects) {
        rect.setPosition(ladder.left, ladder.top);
        rect.setSize({ ladder.width, ladder.height });
        window.draw(rect);
    }

    // Dibujar zonas de colisión (rojo)
    rect.setFillColor(sf::Color(255, 0, 0, 100)); // rojo semi-transparente
    for (const auto& box : ladderBlockers) {
        rect.setPosition(box.left, box.top);
        rect.setSize({ box.width, box.height });
        window.draw(rect);
    }
}

const std::vector<sf::FloatRect>& Level::getCollisionRects() const {
    return collisionRects;
}

const std::vector<sf::FloatRect>& Level::getLadderRects() const {
    return ladderRects;
}

const std::vector<sf::FloatRect>& Level::getLadderBlockersRects() const
{
    return ladderBlockers;
}

void Level::setLadderBlockersEnabled(bool enabled)
{
    ladderBlockersEnabled = enabled;
}

bool Level::getLadderBlockersEnabled() const
{
    return ladderBlockersEnabled;
}



