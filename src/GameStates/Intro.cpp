#include "GameStates/Intro.hpp"
#include "GameStates/Level.hpp"

#include <iostream>

Intro::Intro(sf::RenderWindow& window):
	GameState(window),
	initialZoom(0.5),
	switched(true),
	i(0)
{
	myView = window.getDefaultView();
	window.setView(myView);
}

void Intro::processEvent(sf::Event& event) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed) {
		switched = true;
		++i;
	}
}

void Intro::logic(const sf::Time deltaT) {
	// functions
	if (switched && (i >= 2 || img.loadFromFile("assets/intro/intro_" + std::to_string(i) + ".png") != true)) {
		nextState = std::unique_ptr<GameState> (new Level(window));
		return;
	}
	if (switched) {
		switched = false;
		myView.setSize(img.getSize().x, img.getSize().y);
		if (i == 0) {
			//myView.setCenter(img.getSize().x/2, img.getSize().y*3*(initialZoom/2));
			myView.setCenter(img.getSize().x/2, img.getSize().y/2);
			myView.zoom(initialZoom);
		} else {
			myView.setCenter(img.getSize().x/2, img.getSize().y/2);
		}
	}
	int delay = 300;
	if (i == 0 && myView.getSize().y <= img.getSize().y) {
		myView.zoom(1 + initialZoom/delay);
	} else {
		myView.setCenter(img.getSize().x/2, img.getSize().y/2);
	}
}

void Intro::draw() {
	sf::Sprite sprite(img);
	window.setView(myView);
	window.draw(sprite);
}


