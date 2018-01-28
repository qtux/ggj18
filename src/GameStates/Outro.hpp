#pragma once
#include "../GameState.hpp"
#include <SFML/Graphics.hpp>

#include <string>

class Outro: public GameState {
public:
	Outro(sf::RenderWindow& window);
private:
	void processEvent(sf::Event& event) override;
	void logic(const sf::Time deltaT) override;
	void draw() override;
	sf::Texture img;
	sf::View myView;
};
