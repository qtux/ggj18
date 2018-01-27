#include "Player.hpp"

Player::Player()
{
	myState = PlayerState::NONE;
	myPhysicsBody = 0;
}


void Player::initiatePhysics(b2World* myWorld)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(100, 25);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = myWorld->CreateBody(&bodyDef);
	
	b2PolygonShape shape;
	shape.SetAsBox(32, 32);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.7f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

void Player::Update()
{
	switch(myState)
	{
		break;
	}

}
