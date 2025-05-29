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
        std::string fullPath = tileset.getImagePath();

        if (tex.loadFromFile(fullPath)) {
            tilesetTextures[tileset.getFirstGID()] = std::move(tex);
            std::cout << "Tileset loaded: " << fullPath << "\n";
        }
        else {
            std::cerr << "Failed to load tileset: " << fullPath << "\n";
        }
    }

    for (const auto& layer : map.getLayers()) {
        if (!layer->getVisible()) continue;

        const std::string& name = layer->getName();
        if (layer->getType() == tmx::Layer::Type::Object) {
            const auto& objGroup = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objGroup.getObjects()) {
                auto pos = obj.getPosition();
                auto size = obj.getAABB();
                sf::FloatRect rect(pos.x, pos.y, size.width, size.height);

                if (name == "Collisions") {
                    collisionRects.push_back(rect);
                }
                else if (name == "Ladders") {
                    ladderRects.push_back(rect);
                }
                else if (name == "LaddersBlockers") {
                    ladderBlockers.push_back(rect);
                }
            }
        }
    }

    std::cout << "Loaded map: " << tmxPath << "\n";
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
                sprite.setPosition(static_cast<float>(x * tileSize.x), static_cast<float>(y * tileSize.y));
                window.draw(sprite);
            }
        }
    }
}

const std::vector<sf::FloatRect>& Level::getCollisionRects() const {
    return collisionRects;
}

const std::vector<sf::FloatRect>& Level::getLadderRects() const {
    return ladderRects;
}

const std::vector<sf::FloatRect>& Level::getLadderBlockersRects() const {
    return ladderBlockers;
}

void Level::setLadderBlockersEnabled(bool enabled) {
    ladderBlockersEnabled = enabled;
}

bool Level::getLadderBlockersEnabled() const {
    return ladderBlockersEnabled;
}
