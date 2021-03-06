#pragma once

// third party
#include <Box2D/Box2D.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Object.hpp>
#include <SFML/Graphics.hpp>
#include <lua.hpp>

// engine
#include "../GameState.hpp"
#include "../Player.hpp"
#include "SFMLOrthogonalLayer.hpp"
#include "Deathscreen.hpp"
#include "Outro.hpp"

class Level: public GameState {
public:
	Level(sf::RenderWindow& window, int levelNumber);
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
	int isImmortal = 0;
	sf::CircleShape skillJump, skillFly, skillSlide;
	b2Body* goalBody;
	int levelNumber;
};
