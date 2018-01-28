#include "Entity.hpp"

#include "Settings.hpp"

#include <iostream>

Entity::Entity(std::string texture_file, sf::IntRect texture_rect, sf::Vector2<float> position,
		sf::Vector2<float> size, b2World& world, float density, float friction) {
	// load texture and create a shape
	shape = sf::RectangleShape(size);
	texture.loadFromFile(texture_file);
	shape.setTexture(&texture);
	shape.setTextureRect(texture_rect);
	shape.setPosition(position);
	
	auto scale = Settings::instance()->getProperty<float>("box2d_scale");
	float collider_scale = 0.68;	// hardcoded fix to enable a smaller collision box
	
	// create the shape with a fixture
	b2PolygonShape b2shape;
	b2shape.SetAsBox(size.x*.5f * scale * collider_scale, size.y*.5f * scale * collider_scale);
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &b2shape;
	
	// create a body and add the fixture
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2((position.x) * scale, position.y * scale);
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	body = world.CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

std::pair<bool,bool> Entity::hasContact() {
	// iterate over the Box2d body's contacts
	bool isDead=false, onGround=false;
	b2ContactEdge* edge = nullptr;
	int ct = 0;
	for (edge = body->GetContactList(); edge; edge = edge->next) {
		if (!edge->contact->IsEnabled() || !edge->contact->IsTouching()) {
			continue;
		}
		ct++;
		// std::cout<<"hasContact"<<std::endl;
		// get vector from the body's position to the contact's
		//b2Vec2 v = edge->other->GetPosition() - body->GetPosition();
		
		b2Vec2 norm = edge->contact->GetManifold()->localNormal;

		//std::cout<<"collision #"<<ct<<": v.x = "<<v.x<<", v.y = "<<v.y<<std::endl; 
		//std::cout<<"collision #"<<ct<<": norm.x = "<<norm.x<<", norm.y = "<<norm.y<<std::endl; 
		if ((norm.x) > .1) {
			isDead = true;
		}
		else
		{
			onGround = true;
		}
		//if (!onGround)std::cout<<"hasContact "<<v.x << ","<<v.y<<std::endl;
		/*if (fabs(v.x) > 20) {
			std::cout<<"wallHit"<<std::endl;
			wallHit = true;
		}
		if (fabs(v.y) > 20) {
			std::cout<<"onGround"<<std::endl;*/
			//onGround = true;
		//}
	}
	
	return std::make_pair(onGround, isDead);
	
}

bool Entity::hasWon() {
	b2ContactEdge* edge = nullptr;
	int ct = 0;
	for (edge = body->GetContactList(); edge; edge = edge->next) {
		if (!edge->contact->IsEnabled() || !edge->contact->IsTouching()) {
			continue;
		}
		ct++;
		if (edge->other->GetFixtureList()[0].IsSensor()) {
			return true;
		}
	}
	return false;
}


void Entity::update(float dt) {
	auto scale = Settings::instance()->getProperty<float>("box2d_scale");
	auto body_position = body->GetPosition();
	shape.setPosition(body_position.x / scale - shape.getSize().x / 2, body_position.y / scale - shape.getSize().y / 2);
	
	
    
}

void Entity::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
	renderTarget.draw(shape, states);
}

void Entity::setTextureRect(sf::IntRect rect) {
	shape.setTextureRect(rect);
}

sf::Vector2<float> Entity::getPos() {
	return shape.getPosition();
}

sf::Vector2<float> Entity::getSize() {
	return shape.getSize();
}

void Entity::toggleShape() {
	//std::cout << "toggle" << std::endl;
	b2PolygonShape* b2shape = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()[0].GetShape());
	
	//float collider_scale;
	float scale;
	if (toggle) {
		toggle = false;
		scale = 0.5;
	} else {
		toggle = true;
		scale = 2;
	}
	
	/*for (int i = 0; i < b2shape->m_count; ++i) {
		std::cout << b2shape->m_vertices[i].x << " " << b2shape->m_vertices[i].y << std::endl;
	}*/
	b2shape->m_vertices[2].y *= scale; 
	b2shape->m_vertices[3].y *= scale;
}
