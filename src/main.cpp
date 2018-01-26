#include <SFML/Graphics.hpp>

enum class GameStates {
	GAME_STATE_LEVEL = 0,
	GAME_STATE_HELP
};

int main() {
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Gamename", sf::Style::Default);
	GameStates gameState = GameStates::GAME_STATE_LEVEL;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		switch(gameState) {
			case GameStates::GAME_STATE_HELP:
				// functions
				//window.draw(...);
				break;
			case GameStates::GAME_STATE_LEVEL:
				// functions
				//window.draw(...);
				break;
		}
		window.display();
	}
}
