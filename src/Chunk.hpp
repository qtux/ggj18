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

#pragma once

#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>
#include <vector>
#include <array>

#include "ChunkArray.hpp"

class Chunk final : public sf::Transformable, public sf::Drawable {
public:
	using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
	Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
		const sf::Vector2f& position, const sf::Vector2f& tileCount, std::size_t rowSize,  TextureResource& tr);
	~Chunk() = default;
	Chunk(const Chunk&) = delete;
	Chunk& operator = (const Chunk&) = delete;
	bool empty() const { return m_chunkArrays.empty(); }
private:
	std::vector<std::unique_ptr<ChunkArray>> m_chunkArrays;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};
