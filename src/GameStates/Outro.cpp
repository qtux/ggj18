#include "Outro.hpp"
#include "Level.hpp"

Outro::Outro(sf::RenderWindow& window):	GameState(window) {
	myView = window.getDefaultView();
	window.setView(myView);
	logic(sf::seconds(1.0f / 60.0f));
}

void Outro::processEvent(sf::Event& event) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::JoystickButtonPressed) {
		nextState = std::unique_ptr<GameState>(new Level(window, 0));
	}
}

void Outro::logic(const sf::Time deltaT) {
	if (img.loadFromFile("assets/outro/outro.png") != true) {
		nextState = std::unique_ptr<GameState> (new Level(window, 0));
		return;
	}
	myView.setSize(img.getSize().x, img.getSize().y);
	myView.setCenter(img.getSize().x/2, img.getSize().y/2);
}

void Outro::draw() {
	sf::Sprite sprite(img);
	window.setView(myView);
	window.draw(sprite);
	sf::sleep(sf::milliseconds(300));
}
