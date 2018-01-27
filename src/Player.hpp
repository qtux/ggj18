#pragma once

#include <Box2D/Box2D.h>

enum class PlayerState{
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
	
	/// creates box2d objects for player
	void initiatePhysics(b2World &myWorld); 
	
	public: 
	
	Player(b2World &myWorld);
	
	void ActionTrigger(PlayerState triggeredAction);
	void ActionSwap(PlayerState swappedAction);
	
	void Update();
	
	~Player();
	
	
};
