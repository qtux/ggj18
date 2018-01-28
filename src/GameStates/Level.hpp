#pragma once

// third party
#include <Box2D/Box2D.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Object.hpp>
#include <SFML/Graphics.hpp>

// engine
#include "../GameState.hpp"
#include "../Player.hpp"
#include "../MapLayer.hpp"

class Level: public GameState {
public:
	Level(sf::RenderWindow& window);
	~Level();
private:
	void processEvent(sf::Event& event) override;
	void logic(const sf::Time deltaT) override;
	void draw() override;
	b2PolygonShape createShape(const tmx::Object& obj);
	bool useKeyboard; // use keyboard for second player if not two joysticks (=gamepads) are connected
	b2Vec2 gravity;
	b2World world;
	tmx::Map map;
	Player* playerTop;
	Player* playerBottom;
	sf::View myView;
	MapLayer* layerZero;
	MapLayer* bg;
};
