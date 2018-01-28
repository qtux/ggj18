#pragma once

#include <utility>
#include "Entity.hpp"

enum class PlayerState{
	NONE = 0,
	JUMPING,
	FLYING,
	SHOOTING,
	SLIDING
};

class Player: public Entity {
private:
	std::map<PlayerState,bool> mySkills;
	PlayerState state;
	float timePassed = 0;
	static std::map<PlayerState, std::pair<std::vector<int>, std::vector<float>>> animationMap;
	int animationIndex;
	int animationCounter;
	bool ducked = false;
	void duck();
	void standUp();
	
public:
	Player(b2World& world, bool color, sf::Vector2<float> position  = {100, 100});
	void ActionTrigger(PlayerState triggeredAction);
	void ActionSwap(PlayerState swappedAction);
	void update(float dt=-1.);
	bool hasSkill(PlayerState checkSTate);
	~Player();
};

const static int scaleFactor = 4;
const static int baseSpeed = 400;
