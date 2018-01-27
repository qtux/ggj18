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

#include "Chunk.hpp"

Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
	const sf::Vector2f& position, const sf::Vector2f& tileCount, std::size_t rowSize,  TextureResource& tr) {
	auto opacity = static_cast<sf::Uint8>(layer.getOpacity() /  1.f * 255.f);
	sf::Color vertColour = sf::Color::White;
	vertColour.a = opacity;

	auto offset = layer.getOffset();
	sf::Vector2f layerOffset(offset.x, offset.y);

	const auto& tileIDs = layer.getTiles();
	
	//go through the tiles and create the appropriate arrays
	for (const auto ts : tilesets) {
		bool chunkArrayCreated = false;
		auto tileSize = ts->getTileSize();

		sf::Vector2u tsTileCount;

		std::size_t xPos = static_cast<std::size_t>(position.x / tileSize.x);
		std::size_t yPos = static_cast<std::size_t>(position.y / tileSize.y);

		for (auto y = yPos; y < yPos + tileCount.y; ++y) {
			for (auto x = xPos; x < xPos + tileCount.x; ++x) {
				auto idx = (y * rowSize + x);
				if (idx < tileIDs.size() && tileIDs[idx].ID >= ts->getFirstGID()
					&& tileIDs[idx].ID < (ts->getFirstGID() + ts->getTileCount())) {
					//ID must belong to this set - so add a tile
					if (!chunkArrayCreated) {
						m_chunkArrays.emplace_back(std::make_unique<ChunkArray>(*tr.find(ts->getImagePath())->second));
						auto texSize = m_chunkArrays.back()->getTextureSize();
						tsTileCount.x = texSize.x / tileSize.x;
						tsTileCount.y = texSize.y / tileSize.y;
						chunkArrayCreated = true;
					}
					auto& ca = m_chunkArrays.back();
					sf::Vector2f tileOffset(x * tileSize.x, y * tileSize.y);
					
					auto idIndex = tileIDs[idx].ID - ts->getFirstGID();
					sf::Vector2f tileIndex(idIndex % tsTileCount.x, idIndex / tsTileCount.x);
					tileIndex.x *= tileSize.x;
					tileIndex.y *= tileSize.y;
					std::array<sf::Vertex, 4u> tile = {
						sf::Vertex(tileOffset, vertColour, tileIndex),
						sf::Vertex(tileOffset + sf::Vector2f(tileSize.x, 0.f), vertColour, tileIndex + sf::Vector2f(tileSize.x, 0.f)),
						sf::Vertex(tileOffset + sf::Vector2f(tileSize.x, tileSize.y), vertColour, tileIndex + sf::Vector2f(tileSize.x, tileSize.y)),
						sf::Vertex(tileOffset + sf::Vector2f(0.f, tileSize.y), vertColour, tileIndex + sf::Vector2f(0.f, tileSize.y))
					};
					ca->addTile(tile);
				}
			}
		}
	}
	setPosition(position);
}

void Chunk::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (const auto& a : m_chunkArrays) {
		rt.draw(*a, states);
	}
}
