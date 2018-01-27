#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class GameState {
public:
	GameState(sf::RenderWindow& window): window(window), nextState(nullptr) {};
	~GameState() = default;
	
	// dissallow copy
	GameState(GameState const&) = delete;
	GameState& operator=(GameState const&) = delete;
	
	std::unique_ptr<GameState> run() {
		while (nextState == nullptr && window.isOpen()) {
			
			sf::Event event;
			while (window.pollEvent(event)) {
				processEvent(event);
			}
			float timeStep = 1.0f / 60.0f;	// TODO use config value
			if (clock.getElapsedTime() > sf::seconds(timeStep)) {
				sf::Time deltaT = clock.restart();
				logic(deltaT);
			}
			window.clear(sf::Color::Black);
			draw();
			window.display();
		}
		return std::move(nextState);
	};
protected:
	virtual void processEvent(sf::Event& event) = 0;
	virtual void logic(const sf::Time deltaT) = 0;
	virtual void draw() = 0;
	sf::Clock clock;
	sf::RenderWindow& window;
	std::unique_ptr<GameState> nextState;
};
