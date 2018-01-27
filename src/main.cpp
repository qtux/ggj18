#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include "MapLayer.hpp"
#include <Box2D/Box2D.h>

enum class GameStates {
	GAME_STATE_LEVEL = 0,
	GAME_STATE_HELP
};

int main() {
	sf::RenderWindow window(sf::VideoMode(1024, 768), "SkillSwitch", sf::Style::Default);
	
	// test tmxlite
	tmx::Map map;
	map.load("assets/levels/level1.tmx");
	MapLayer layerZero(map, 0);
	
	// test sprite
	sf::Texture texture;
	texture.loadFromFile("assets/sprites/level.png");
	sf::Sprite test;
	test.setTexture(texture);
	test.setTextureRect(sf::IntRect(10, 10, 50, 30));
	test.setColor(sf::Color(255, 255, 255, 200));
	test.setPosition(100, 25);
	
	// test Box2D
	b2Vec2 gravity(0.f, 9.8f);
	b2World world(gravity);
	
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(100, 25);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = world.CreateBody(&bodyDef);
	
	b2PolygonShape shape;
	shape.SetAsBox(32, 32);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.7f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
	
	GameStates gameState = GameStates::GAME_STATE_LEVEL;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		switch(gameState) {
			case GameStates::GAME_STATE_HELP:
				// functions
				//window.draw(...);
				break;
			case GameStates::GAME_STATE_LEVEL:
				// functions
				//window.draw(...);
				window.draw(layerZero);
				window.draw(test);
				world.Step(1/60.f, 8, 3);
				test.setPosition(body->GetPosition().x, body->GetPosition().y);
				break;
		}
		window.display();
	}
}
