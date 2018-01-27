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
