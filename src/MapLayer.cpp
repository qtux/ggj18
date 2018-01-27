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

#include "MapLayer.hpp"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>
#include <map>
#include <limits>
#include <iostream>
#include <cmath>

MapLayer::MapLayer(const tmx::Map& map, std::size_t idx) {
	const auto& layers = map.getLayers();
	if (map.getOrientation() == tmx::Orientation::Orthogonal &&
		idx < layers.size() && layers[idx]->getType() == tmx::Layer::Type::Tile) {
		//round the chunk size to the nearest tile
		const auto tileSize = map.getTileSize();
		m_chunkSize.x = std::floor(m_chunkSize.x / tileSize.x) * tileSize.x;
		m_chunkSize.y = std::floor(m_chunkSize.y / tileSize.y) * tileSize.y;
		
		const auto& layer = *dynamic_cast<const tmx::TileLayer*>(layers[idx].get());
		createChunks(map, layer);
		
		auto mapSize = map.getBounds();
		m_globalBounds.width = mapSize.width;
		m_globalBounds.height = mapSize.height;
	}
	else {
		std::cout << "Not a valid othogonal layer, nothing will be drawn." << std::endl;
	}
}

MapLayer::Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
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
					Tile tile = {
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

void MapLayer::Chunk::ChunkArray::addTile(const Chunk::Tile& tile) {
	for (const auto& v : tile) {
		m_vertices.push_back(v);
	}
}

void MapLayer::Chunk::ChunkArray::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.texture = &m_texture;
	rt.draw(m_vertices.data(), m_vertices.size(), sf::Quads, states);
}

void MapLayer::Chunk::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (const auto& a : m_chunkArrays) {
		rt.draw(*a, states);
	}
}

void MapLayer::createChunks(const tmx::Map& map, const tmx::TileLayer& layer) {
	//look up all the tile sets and load the textures
	const auto& tileSets = map.getTilesets();
	const auto& layerIDs = layer.getTiles();
	std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
	std::vector<const tmx::Tileset*> usedTileSets;

	for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i) {
		for (const auto& tile : layerIDs) {
			if (tile.ID >= i->getFirstGID() && tile.ID < maxID) {
				usedTileSets.push_back(&(*i));
				break;
			}
		}
		maxID = i->getFirstGID();
	}

	sf::Image fallback;
	fallback.create(2, 2, sf::Color::Magenta);
	for (const auto ts : usedTileSets) {
		const auto& path = ts->getImagePath();
		std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
		sf::Image img;
		if (!img.loadFromFile(path)) {
			newTexture->loadFromImage(fallback);
		}
		else {
			if (ts->hasTransparency()) {
				auto transparency = ts->getTransparencyColour();
				img.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
			}
			newTexture->loadFromImage(img);
		}
		m_textureResource.insert(std::make_pair(path, std::move(newTexture)));
	}

	//calculate the number of chunks in the layer
	//and create each one
	const auto bounds = map.getBounds();
	m_chunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / m_chunkSize.x));
	m_chunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / m_chunkSize.y));

	sf::Vector2f tileCount(m_chunkSize.x / map.getTileSize().x, m_chunkSize.y / map.getTileSize().y);

	for (auto y = 0u; y < m_chunkCount.y; ++y) {
		for (auto x = 0u; x < m_chunkCount.x; ++x) {
			m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
				sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount, map.getTileCount().x, m_textureResource));
		}
	}
}

void MapLayer::updateVisibility(const sf::View& view) const {
		sf::Vector2f viewCorner = view.getCenter();
		viewCorner -= view.getSize() / 2.f;
		
		int posX = static_cast<int>(std::floor(viewCorner.x / m_chunkSize.x));
		int posY = static_cast<int>(std::floor(viewCorner.y / m_chunkSize.y));
		
		std::vector<const Chunk*> visible;
		for (auto y = posY; y < posY + 2; ++y) {
			for (auto x = posX; x < posX + 2; ++x) {
				auto idx = y * int(m_chunkCount.x) + x;
				if (idx >= 0 && idx < m_chunks.size() && !m_chunks[idx]->empty()) {
					visible.push_back(m_chunks[idx].get());
				}
			}
		}
		std::swap(m_visibleChunks, visible);
	}

void MapLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	//calc view coverage and draw nearest chunks
	updateVisibility(rt.getView());
	for (const auto& c : m_visibleChunks) {
		rt.draw(*c, states);
	}
}
