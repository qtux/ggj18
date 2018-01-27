#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include <string>

class Entity: public sf::Drawable {
public:
	Entity(std::string texture_file, sf::IntRect texture_rect, sf::Vector2<float> position, sf::Vector2<float> size, b2World& world);
	void update();
private:
	sf::IntRect texture_rect;
	sf::RectangleShape shape;
	sf::Vector2<float> position;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	b2Body* body;
};
