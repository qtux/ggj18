#pragma once
#include "../GameState.hpp"
#include <SFML/Graphics.hpp>

class Intro: public GameState {
public:
	Intro(sf::RenderWindow& window);
private:
	void processEvent(sf::Event& event) override;
	void logic(const sf::Time deltaT) override;
	void draw() override;
	float initialZoom;
	bool switched;
	int i;
	sf::Texture img;
	sf::View myView;
};
