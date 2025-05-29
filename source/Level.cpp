#include "Level.h"
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

    tilesetTextures.clear();

    for (const auto& tileset : map.getTilesets()) {
        sf::Texture tex;
        std::string base = "";
        std::string fullPath = base + tileset.getImagePath();
        if (tex.loadFromFile(fullPath)) {
            tilesetTextures[tileset.getFirstGID()] = std::move(tex);
            std::cout << "Cargado tileset: " << fullPath << "\n";
        }
        else {
            std::cerr << "Error al cargar tileset: " << fullPath << "\n";
        }
    }


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
    const auto& mapSize = map.getTileCount();
    const auto& tileSize = map.getTileSize();

    for (const auto& layer : map.getLayers()) {
        if (layer->getType() != tmx::Layer::Type::Tile || !layer->getVisible())
            continue;

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles();

        for (std::size_t y = 0; y < mapSize.y; ++y) {
            for (std::size_t x = 0; x < mapSize.x; ++x) {
                std::size_t index = x + y * mapSize.x;
                std::uint32_t tileID = tiles[index].ID;

                if (tileID == 0) continue;

                const sf::Texture* texture = nullptr;
                std::size_t localID = tileID;

                for (auto it = tilesetTextures.rbegin(); it != tilesetTextures.rend(); ++it) {
                    if (tileID >= it->first) {
                        texture = &it->second;
                        localID = tileID - it->first;
                        break;
                    }
                }

                if (!texture) continue;

                std::size_t columns = texture->getSize().x / tileSize.x;
                std::size_t tu = localID % columns;
                std::size_t tv = localID / columns;

                sf::Sprite sprite;
                sprite.setTexture(*texture);
                sprite.setTextureRect({
                    static_cast<int>(tu * tileSize.x),
                    static_cast<int>(tv * tileSize.y),
                    static_cast<int>(tileSize.x),
                    static_cast<int>(tileSize.y)
                    });
                sprite.setPosition(static_cast<float>(x * tileSize.x),
                    static_cast<float>(y * tileSize.y));

                window.draw(sprite);
            }
        }
    }

    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.f);

   /* for (const auto& r : getCollisionRects()) {
        sf::RectangleShape debugRect({ r.width, r.height });
        debugRect.setPosition(r.left, r.top);
        debugRect.setFillColor(sf::Color(255, 255, 0, 100)); // amarillo translúcido
        window.draw(debugRect);
    }*/


   /* rect.setFillColor(sf::Color(255, 0, 0, 100)); // colisiones
    for (const auto& box : collisionRects) {
        rect.setPosition(box.left, box.top);
        rect.setSize({ box.width, box.height });
        window.draw(rect);
    }

    rect.setFillColor(sf::Color(0, 255, 0, 100)); // escaleras
    for (const auto& ladder : ladderRects) {
        rect.setPosition(ladder.left, ladder.top);
        rect.setSize({ ladder.width, ladder.height });
        window.draw(rect);
    }

    if (ladderBlockersEnabled) {
        rect.setFillColor(sf::Color(255, 0, 255, 100)); // blockers
        for (const auto& lb : ladderBlockers) {
            rect.setPosition(lb.left, lb.top);
            rect.setSize({ lb.width, lb.height });
            window.draw(rect);
        }
    }*/
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



