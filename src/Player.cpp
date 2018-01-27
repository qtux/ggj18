#include "Player.hpp"

Player::Player(b2World& world): Entity("assets/sprites/dodo.png", sf::IntRect(0, 0, 64, 64), {100, 100}, {64, 64}, world) {
	state = PlayerState::NONE;
}

//Player::~Player(){}

void Player::update() {
	switch(state) {
		case PlayerState::NONE:
			this->setTextureRect(sf::IntRect(0*64, 0*64+64, 64, 64));
			break;
	}

}

