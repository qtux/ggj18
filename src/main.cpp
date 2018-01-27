#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include "MapLayer.hpp"
#include <Box2D/Box2D.h>
#include <string>
#include <iostream>

#include "Player.hpp"

enum class GameStates {
	GAME_STATE_LEVEL = 0,
	GAME_STATE_HELP
};

int main() {
	sf::RenderWindow window(sf::VideoMode(1024, 768), "SkillSwitch", sf::Style::Default);
	sf::View myView = window.getDefaultView();
	window.setView(myView);
	
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
	
	// show intro
	int i = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed)
				++i;
		}
		window.clear(sf::Color::Black);
		sf::Texture img;
		if(i >= 2 || img.loadFromFile("assets/intro/intro_" + std::to_string(i) + ".png") != true) {
			break;
		}
		sf::Sprite sprite(img);
		myView.setSize(img.getSize().x, img.getSize().y);
		myView.setCenter(img.getSize().x/2, img.getSize().y/2);
		window.setView(myView);
		window.draw(sprite);
		window.display();
	}
	window.setView(window.getDefaultView());
	
	// the game view (full window)
//myView.setViewport(sf::FloatRect(0, 0, , 768./1280.));
	//myView.zoom(1280./768.);
	myView.setSize(1707,1280);//1707 = aspect ratio * 1280
	
	
	// clock to determine fixed logic rate
	sf::Clock clock;
	float timeStep = 1.0f / 60.0f;
	//start game
	GameStates gameState = GameStates::GAME_STATE_LEVEL;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		window.setView(myView);
		
		// fixed rate for logic functions
		if (clock.getElapsedTime() > sf::seconds(timeStep)) {
			// reset clock if more than a 1/60 seconds have passed
			sf::Time deltaT = clock.restart();
			switch(gameState) {
				case GameStates::GAME_STATE_HELP:
					// functions
					break;
				case GameStates::GAME_STATE_LEVEL:
					// functions
					world.Step(deltaT.asSeconds(), 8, 3);
					test.setPosition(body->GetPosition().x, body->GetPosition().y);
					myView.setCenter(myView.getCenter().x + 600 * deltaT.asSeconds(), 1280./2);
					break;
			}
		}
		// draw as often as possible
		switch(gameState) {
			case GameStates::GAME_STATE_HELP:
				break;
			case GameStates::GAME_STATE_LEVEL:
				window.draw(layerZero);
				window.draw(test);
				break;
		}
		//myView.display();
		window.display();
	}
}
