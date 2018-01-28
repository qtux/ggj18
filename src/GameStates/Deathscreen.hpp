#pragma once
#include "../GameState.hpp"
#include <SFML/Graphics.hpp>

#include <string>

class Deathscreen: public GameState {
public:
	Deathscreen(sf::RenderWindow& window);
private:
	void processEvent(sf::Event& event) override;
	void logic(const sf::Time deltaT) override;
	void draw() override;
	sf::Texture img;
	int index;
	std::vector<std::string> imgs;
	sf::View myView;
};
