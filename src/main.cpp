#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include "MapLayer.hpp"
#include <Box2D/Box2D.h>
#include <string>
#include <iostream>

#include "Player.hpp"
#include "Entity.hpp"
#include "CollisionLayer.hpp"

enum class GameStates {
	GAME_STATE_INTRO = 0,
	GAME_STATE_LEVEL
};

int main() {
	sf::RenderWindow window(sf::VideoMode(1024, 768), "SkillSwitch", sf::Style::Default);
	sf::View myView = window.getDefaultView();
	window.setView(myView);
	
	// test tmxlite
	tmx::Map map;
	map.load("assets/levels/level1.tmx");
	MapLayer layerZero(map, 0);
	
	// test Box2D
	b2Vec2 gravity(0.f, 9.8f);
	b2World world(gravity);
	CollisionLayer collision_layer(map, world);
	Entity test("assets/sprites/level.png", sf::IntRect(10, 10, 50, 30), {100, 25}, {32, 32}, world);
	
	// the game view (full window)
	//myView.setViewport(sf::FloatRect(0, 0, , 768./1280.));
	//myView.zoom(1280./768.);
	myView.setSize(1707,1280);//1707 = aspect ratio * 1280
	
	
	// clock to determine fixed logic rate
	sf::Clock clock;
	float timeStep = 1.0f / 60.0f;
	int i = 0;
	bool switched = true;
	sf::Texture img;
	//start game
	GameStates gameState = GameStates::GAME_STATE_INTRO;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed) {
				switched = true;
				++i;
			}
		}
		window.clear(sf::Color::Black);
		window.setView(myView);
		
		// fixed rate for logic functions
		float initialZoomIntro = 0.5;
		if (clock.getElapsedTime() > sf::seconds(timeStep)) {
			// reset clock if more than a 1/60 seconds have passed
			sf::Time deltaT = clock.restart();
			switch(gameState) {
				case GameStates::GAME_STATE_INTRO: {
					// functions
					if (switched && (i >= 2 || img.loadFromFile("assets/intro/intro_" + std::to_string(i) + ".png") != true)) {
						gameState = GameStates::GAME_STATE_LEVEL;
						break;
					}
					if (switched) {
						switched = false;
						myView.setSize(img.getSize().x, img.getSize().y);
						if (i == 0) {
							//myView.setCenter(img.getSize().x/2, img.getSize().y*3*(initialZoomIntro/2));
							myView.setCenter(img.getSize().x/2, img.getSize().y/2);
							myView.zoom(initialZoomIntro);
						} else {
							myView.setCenter(img.getSize().x/2, img.getSize().y/2);
						}
					}
					int delay = 300;
					if (i == 0 && myView.getSize().y <= img.getSize().y) {
						myView.zoom(1 + initialZoomIntro/delay);
					} else {
						myView.setCenter(img.getSize().x/2, img.getSize().y/2);
					}
					break;
				}
				case GameStates::GAME_STATE_LEVEL:
					// functions
					world.Step(deltaT.asSeconds(), 8, 3);
					test.update();
					myView.setCenter(myView.getCenter().x + 600 * deltaT.asSeconds(), 1280./2);
					break;
			}
		}
		// draw as often as possible
		switch(gameState) {
			case GameStates::GAME_STATE_INTRO: {
				sf::Sprite sprite(img);
				window.setView(myView);
				window.draw(sprite);
				break;
			}
			case GameStates::GAME_STATE_LEVEL: {
				window.draw(layerZero);
				window.draw(test);
				break;
			}
		}
		//myView.display();
		window.display();
	}
}
