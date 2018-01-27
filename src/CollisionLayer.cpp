#include "CollisionLayer.hpp"

#include <iostream>
#include <vector>

CollisionLayer::CollisionLayer(const tmx::Map& map, b2World& world) {
	for (auto& layer:map.getLayers()) {
		std::cout << layer->getName() << std::endl;
		if (map.getOrientation() == tmx::Orientation::Orthogonal &&
			layer->getType() == tmx::Layer::Type::Object && layer->getName() == "Collision") {
			auto collision_layer = *dynamic_cast<const tmx::ObjectGroup*>(layer.get());
			for (auto& obj: collision_layer.getObjects()) {
				// create the shape with a fixture
				b2PolygonShape shape = createShape(obj);
				b2FixtureDef fixtureDef;
				fixtureDef.density = 1.f;
				fixtureDef.friction = 0.7f;
				fixtureDef.shape = &shape;
				// create a body and add the fixture
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				b2Body* body = world.CreateBody(&bodyDef);
				body->CreateFixture(&fixtureDef);
			}
			return;
		}
	}
	std::cout << "The collision layer could not be loaded." << std::endl;
}

b2PolygonShape CollisionLayer::createShape(const tmx::Object& obj) {
	switch(obj.getShape()) {
		case tmx::Object::Shape::Rectangle: {
			auto aabb = obj.getAABB();
			b2PolygonShape shape;
			shape.SetAsBox(
				aabb.width * 0.5,
				aabb.height * 0.5,
				b2Vec2(aabb.left + aabb.width * 0.5, aabb.top + aabb.height * 0.5)
				, 0
			);
			return shape;
		}
		
		case tmx::Object::Shape::Polygon: {
			auto points = obj.getPoints();
			auto pos = obj.getPosition();
			b2PolygonShape shape;
			std::vector<b2Vec2> b2points;
			b2points.reserve(points.size());
			for (auto& point: points) {
				b2points.push_back(b2Vec2(pos.x + point.x, pos.y + point.y));
			}
			shape.Set(b2points.data(), b2points.size());
			return shape;
		}
		default: {
			break;
			// TODO implement tmx::Object::Shape::Ellipse and tmx::Object::Shape::Polyline
		}
		
	}
	return b2PolygonShape();
}
