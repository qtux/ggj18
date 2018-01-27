#pragma once

#include <Box2D/Box2D.h>
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
	PlayerState state;
	static std::map<PlayerState, std::vector<int>> animationMap;
public:
	Player(b2World& world);
	void ActionTrigger(PlayerState triggeredAction);
	void ActionSwap(PlayerState swappedAction);
	void update(float dt=-1.);
	~Player();
};

const static int scaleFactor = 4;
