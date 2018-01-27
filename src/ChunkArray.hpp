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

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>
#include <map>

class ChunkArray final : public sf::Drawable {
public:
	explicit ChunkArray(const sf::Texture& t): m_texture(t) {}
	~ChunkArray() = default;
	ChunkArray(const ChunkArray&) = delete;
	ChunkArray& operator = (const ChunkArray&) = delete;
	void addTile(const std::array<sf::Vertex, 4u>& tile);
	sf::Vector2u getTextureSize() const {return m_texture.getSize();}
private:
	const sf::Texture& m_texture;
	std::vector<sf::Vertex> m_vertices;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};
