#pragma once

#include <Box2D/Box2D.h>

enum PlayerState{
	NONE,
	JUMPING,
	FLYING,
	SHOOTING,
	SLIDING
};


class Player
{
	private:
	b2Body* myPhysicsBody;
	PlayerState myState;
	
	public: 
	
	Player();
	
	void Update();
	
	~Player();
	
	
};
