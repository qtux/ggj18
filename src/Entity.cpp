#include "Entity.hpp"

Entity::Entity(std::string texture_file, sf::IntRect texture_rect, sf::Vector2<float> position, sf::Vector2<float> size, b2World& world) {
	// load texture and create a shape
	shape = sf::RectangleShape(size);
	texture.loadFromFile(texture_file);
	shape.setTexture(&texture);
	shape.setTextureRect(texture_rect);
	shape.setPosition(position);
	
	// create the shape with a fixture
	b2PolygonShape shape;
	shape.SetAsBox(size.x, size.y-38); // fixes offset w.r.t. ground
	b2FixtureDef fixtureDef;
	fixtureDef.density = 100.f;
	fixtureDef.friction = 0.0f;
	fixtureDef.shape = &shape;
	
	// create a body and add the fixture
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(position.x, position.y);
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	body = world.CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void Entity::update(float dt) {
	auto body_position = body->GetPosition();
	shape.setPosition(body_position.x, body_position.y);
}

void Entity::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const {
	renderTarget.draw(shape, states);
}

void Entity::setTextureRect(sf::IntRect rect) {
	shape.setTextureRect(rect);
}
