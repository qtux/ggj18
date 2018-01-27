#include "Player.hpp"
#include "Settings.hpp"

std::map<PlayerState, std::vector<int>> Player::animationMap = {
   {PlayerState::NONE, {3, 1, 3, 2, 3}},
   {PlayerState::SHOOTING, {3, 1, 3, 2, 3}},
   {PlayerState::SLIDING, {0, 1, 0}},
   {PlayerState::FLYING, {0, 1, 2, 1, 2, 3}},
   {PlayerState::JUMPING, {0, 1, 2, 3, 4, 3}}
};

Player::Player(b2World& world): Entity("assets/sprites/dodo.png", sf::IntRect(0, 0, 64, 64), {100, 100}, {64*scaleFactor, 64*scaleFactor}, world) {
	state = PlayerState::NONE;
	body->SetLinearVelocity( b2Vec2(Settings::instance()->getProperty<float>("level_speed"),0));
}

void Player::ActionSwap(PlayerState myState){}

void Player::ActionTrigger(PlayerState myState){}

Player::~Player(){}

void Player::update(float dt) {
	Entity::update(dt);
	switch(state) {
		case PlayerState::NONE:
			this->setTextureRect(sf::IntRect(0*64, 0*64, 64, 64));
			break;
	}

} 

