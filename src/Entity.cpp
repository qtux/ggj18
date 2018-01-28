#include "Entity.hpp"

#include "../Settings.hpp"

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

bool Entity::hasContact() {
	// iterate over the Box2d body's contacts
	b2ContactEdge* edge = nullptr;
	int ct = 0;
	for (edge = body->GetContactList(); edge; edge = edge->next) {
		if (!edge->contact->IsEnabled() || !edge->contact->IsTouching()) {
			continue;
		}
		ct++;
		// std::cout<<"hasContact"<<std::endl;
		// get vector from the body's position to the contact's
		b2Vec2 v = edge->other->GetPosition() - body->GetPosition();
		b2Vec2 norm = edge->contact->GetManifold()->localNormal;

		//std::cout<<"collision #"<<ct<<": v.x = "<<v.x<<", v.y = "<<v.y<<std::endl; 
		//std::cout<<"collision #"<<ct<<": norm.x = "<<norm.x<<", norm.y = "<<norm.y<<std::endl; 
		if (fabs(norm.x) > .1) {
			std::cout<<"dead"<<std::endl;
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
	if (ct>0) {
		 return true;
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
