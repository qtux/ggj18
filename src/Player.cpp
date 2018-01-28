#include "Player.hpp"
#include "Settings.hpp"

// the first vector is the number of the sprite (column), second is the delay or speed for each
// vectors must be equal size!
std::map<PlayerState, std::pair<std::vector<int>, std::vector<float>>> Player::animationMap = {
   {PlayerState::NONE, {{3, 1, 3, 2, 3},{baseSpeed/2, baseSpeed, baseSpeed, baseSpeed, baseSpeed/2}}},
   {PlayerState::SHOOTING, {{3, 1, 3, 2, 3},{baseSpeed*2, baseSpeed*2, baseSpeed*2, baseSpeed*2, baseSpeed*2}}},
   {PlayerState::SLIDING, {{0, 1, 2},{baseSpeed*2, baseSpeed*3, baseSpeed*2}}},
   {PlayerState::FLYING, {{0, 1, 2, 1, 2, 1, 2, 3},{baseSpeed*2, baseSpeed*3, baseSpeed*3, baseSpeed*3, baseSpeed*3, baseSpeed*3, baseSpeed*3, baseSpeed*2}}},
   {PlayerState::JUMPING, {{0, 1, 2, 3, 4, 3},{baseSpeed*2, baseSpeed*2, baseSpeed*2, baseSpeed*2, baseSpeed*2, baseSpeed*2}}}
};

Player::Player(b2World& world, bool color, sf::Vector2<float> position):
	Entity("assets/sprites/dodo.png",
		sf::IntRect(0, 0, 64, 64),
		position,
		{64*scaleFactor, 64*scaleFactor},
		world,
		Settings::instance()->getProperty<float>("player_density"),
		Settings::instance()->getProperty<float>("player_friction")
	)
{
	state = PlayerState::NONE;
	animationIndex = 0;
	animationCounter = 0;
	if (color) {
		shape.setFillColor(sf::Color(164, 116, 74));
	}
	mySkills =  {{PlayerState::SHOOTING, false}, {PlayerState::SLIDING, false}, {PlayerState::FLYING, false}, {PlayerState::JUMPING, false}};
}

void Player::ActionSwap(PlayerState myState){
	mySkills[myState] = !mySkills[myState];
	}

void Player::ActionTrigger(PlayerState myState){
	switch(myState)
	{
		case PlayerState::JUMPING:
			if (mySkills[PlayerState::JUMPING] &&  hasContact().first)
			{
				body->ApplyLinearImpulse(b2Vec2(0, Settings::instance()->getProperty<float>("jump_impulse")),body->GetWorldCenter(),true);
				this->state = myState;
				timePassed = 0;
				//this->onGround = false;
			}
			break;
		case PlayerState::FLYING:
			if (hasSkill(PlayerState::FLYING))
			{
				body->SetGravityScale( Settings::instance()->getProperty<float>("flying_gravity_scale") );
				this->state = myState;
				timePassed = 0;
			}
			break;
		case PlayerState::SLIDING:
		std::cout<<"ducking"<<std::endl;
			if (hasSkill(PlayerState::SLIDING))
			{
				if (ducked) 
				{
					standUp();
					this->state = PlayerState::NONE;
				}
				else
				{
					duck();
					this->state = PlayerState::SLIDING;
				}
			}
	}
}

Player::~Player(){}

void Player::duck()
{
	if (!ducked)
	{
		this->toggleShape();
		ducked = true;
	}
}

bool Player::hasSkill(PlayerState checkSTate)
{
	return mySkills[checkSTate];
}


void Player::standUp(){
	if (ducked)
	{
		this->toggleShape();
		ducked = false;
	}
};


void Player::update(float dt) {
	Entity::update(dt);
	timePassed +=dt;
	// change animation after delaying it by the value given by a counter
	if (animationCounter >= animationMap[state].second[animationIndex] * dt) {
		++animationIndex;
		animationCounter = 0;
		if (animationIndex >= animationMap[state].first.size()) {
			animationIndex = 0;
			
			//std::cout<<"never gonna give you up"<<std::endl;
			//state = PlayerState::NONE;
		}
	}
	float v_y = body->GetLinearVelocity().y;
	body->SetLinearVelocity( b2Vec2(Settings::instance()->getProperty<float>("level_speed"),v_y));
	// TODO fix positions
	switch(state) {
		case PlayerState::NONE:
			this->setTextureRect(sf::IntRect(animationMap[state].first[animationIndex]*64, 0, 64, 64));
			break;
		case PlayerState::SHOOTING:
			this->setTextureRect(sf::IntRect(animationMap[state].first[animationIndex]*64, 1*64, 64, 64));
			break;
		case PlayerState::SLIDING:
			if (!hasSkill(PlayerState::SLIDING)) 
			{
				standUp();
				state = PlayerState::NONE;
			}
			std::cout<<"SLIDING!!!!"<<std::endl;
			this->setTextureRect(sf::IntRect(animationMap[state].first[animationIndex]*64, 2*64, 64, 64));
			break;
		case PlayerState::FLYING:
			this->setTextureRect(sf::IntRect(animationMap[state].first[animationIndex]*64, 3*64, 64, 64));
			std::cout<<"flyings"<<std::endl;
			if (hasContact().first && timePassed > .2)
			{
				state = PlayerState::NONE;
				body->SetGravityScale(1);
				std::cout<<"fullgrav"<<std::endl;
			}
			break;
		case PlayerState::JUMPING:
			this->setTextureRect(sf::IntRect(animationMap[state].first[animationIndex]*64, 4*64, 64, 64));
			if (hasContact().first && timePassed > .2)
			{
				state = PlayerState::NONE;
			}
			break;
	}
	++animationCounter;
}
