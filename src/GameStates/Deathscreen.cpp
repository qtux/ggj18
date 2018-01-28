#include "Deathscreen.hpp"
#include "Level.hpp"

Deathscreen::Deathscreen(sf::RenderWindow& window):	GameState(window) {
	imgs = {"assets/death/human.png", "assets/death/human.png", "assets/death/death.png"};
	index = 0;
	myView = window.getDefaultView();
	window.setView(myView);
	logic(sf::seconds(1.0f / 60.0f));
}

void Deathscreen::processEvent(sf::Event& event) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
}

void Deathscreen::logic(const sf::Time deltaT) {
	if (index >= imgs.size() || img.loadFromFile(imgs[index]) != true) {
		nextState = std::unique_ptr<GameState> (new Level(window));
		return;
	}
	myView.setSize(img.getSize().x, img.getSize().y);
	myView.setCenter(img.getSize().x/2, img.getSize().y/2);
	++index;
}

void Deathscreen::draw() {
	sf::Sprite sprite(img);
	window.setView(myView);
	window.draw(sprite);
	sf::sleep(sf::milliseconds(250));
}
