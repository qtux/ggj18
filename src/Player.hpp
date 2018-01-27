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
public:
	Player(b2World& world);
	void ActionTrigger(PlayerState triggeredAction);
	void ActionSwap(PlayerState swappedAction);
	void update(float dt=-1.);
	~Player();
};

const static int none[] = {0};
const static int walk[] = {3, 1, 3, 2, 3};
const static int shoot[] = {3, 1, 3, 2, 3};
const static int slide[] = {0, 1, 0};
const static int fly[] = {0, 1, 2, 1, 2, 3};
const static int jump[] = {0, 1, 2, 3, 4, 3};
