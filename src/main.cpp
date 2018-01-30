#include <SFML/Graphics.hpp>

#include <string>
#include <memory>
#include "Settings.hpp"

#include "GameStates/Intro.hpp"

int main() {
	std::srand(std::time(nullptr));
	
	sf::RenderWindow window(
		sf::VideoMode(Settings::instance()->getProperty<int>("screen_width"),
		Settings::instance()->getProperty<int>("screen_height")),
		Settings::instance()->getProperty<std::string>("name"),
		sf::Style::Default
	);
	
	std::unique_ptr<GameState> state(new Intro(window));
	
	while(state != nullptr) {
		state = state->run();
	}
	//window.setView(myView);
}
