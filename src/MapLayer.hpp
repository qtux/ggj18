/*********************************************************************
Matt Marchant 2016
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

/*
Creates an SFML drawable from an Orthogonal tmx map layer.
This is an example of drawing with SFML - not all features,
such as tile flipping, are implemented. For a more detailed
implementation, including artifact prevention, see:
https://github.com/fallahn/xygine/blob/master/xygine/src/components/ComponentTileMapLayer.cpp
*/

#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>
#include <vector>
#include <array>
#include <map>
#include <string>

class MapLayer final: public sf::Drawable {
public:
    MapLayer(const tmx::Map& map, std::size_t idx);
    ~MapLayer() = default;
    MapLayer(const MapLayer&) = delete;
    MapLayer& operator = (const MapLayer&) = delete;
    const sf::FloatRect& getGlobalBounds() const {return m_globalBounds;}
private:
    sf::Vector2f m_chunkSize = sf::Vector2f(1024.f, 1024.f);
    sf::Vector2u m_chunkCount;
    sf::FloatRect m_globalBounds;
    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
    TextureResource m_textureResource;

    class Chunk final : public sf::Transformable, public sf::Drawable {
    public:
        using Ptr = std::unique_ptr<Chunk>;
        using Tile = std::array<sf::Vertex, 4u>;
        Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
            const sf::Vector2f& position, const sf::Vector2f& tileCount, std::size_t rowSize,  TextureResource& tr);
        ~Chunk() = default;
        Chunk(const Chunk&) = delete;
        Chunk& operator = (const Chunk&) = delete;
        bool empty() const { return m_chunkArrays.empty(); }
    private:
        class ChunkArray final : public sf::Drawable {
        public:
            using Ptr = std::unique_ptr<ChunkArray>;
            explicit ChunkArray(const sf::Texture& t): m_texture(t) {}
            ~ChunkArray() = default;
            ChunkArray(const ChunkArray&) = delete;
            ChunkArray& operator = (const ChunkArray&) = delete;
            void addTile(const Chunk::Tile& tile);
            sf::Vector2u getTextureSize() const {return m_texture.getSize();}
        private:
            const sf::Texture& m_texture;
            std::vector<sf::Vertex> m_vertices;
            void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
        };
        std::vector<ChunkArray::Ptr> m_chunkArrays;
        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };

    std::vector<Chunk::Ptr> m_chunks;
    mutable std::vector<const Chunk*> m_visibleChunks;
    void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);
    void updateVisibility(const sf::View& view) const;
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};
