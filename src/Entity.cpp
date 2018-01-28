#include "Entity.hpp"

#include "../Settings.hpp"

Entity::Entity(std::string texture_file, sf::IntRect texture_rect, sf::Vector2<float> position, sf::Vector2<float> size, b2World& world) {
	// load texture and create a shape
	shape = sf::RectangleShape(size);
	texture.loadFromFile(texture_file);
	shape.setTexture(&texture);
	shape.setTextureRect(texture_rect);
	shape.setPosition(position);
	
	auto scale = Settings::instance()->getProperty<float>("box2d_scale");
	
	// create the shape with a fixture
	b2PolygonShape b2shape;
	b2shape.SetAsBox(size.x*.5f * scale, size.y*.5f * scale); // fixes offset w.r.t. ground
	b2FixtureDef fixtureDef;
	fixtureDef.density = 100.f;
	fixtureDef.friction = 0.0f;
	fixtureDef.shape = &b2shape;
	
	// create a body and add the fixture
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2((position.x) * scale, position.y * scale);
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	body = world.CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void Entity::update(float dt) {
	auto scale = Settings::instance()->getProperty<float>("box2d_scale");
	auto body_position = body->GetPosition();
	shape.setPosition(body_position.x / scale-128, body_position.y / scale-128);
}

void Entity::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
	renderTarget.draw(shape, states);
}

void Entity::setTextureRect(sf::IntRect rect) {
	shape.setTextureRect(rect);
}
