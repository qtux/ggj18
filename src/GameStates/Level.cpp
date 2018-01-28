#include "GameStates/Level.hpp"

#include <tmxlite/ObjectGroup.hpp>

#include "../Settings.hpp"

#include <iostream>

#include "Box2DDebugDrawer.hpp"

Level::Level(sf::RenderWindow& window):
	GameState(window),
	useKeyboard(!sf::Joystick::isConnected(1)),
	gravity(b2Vec2(0.f, 9.8f)),
	world(b2World(gravity)),
	map()
{
	map.load("assets/levels/level0.tmx");
	  //at global scope
   b2Draw *fooDrawInstance = new FooDraw();
  
  //in constructor, usually
  world.SetDebugDraw( fooDrawInstance );
  
  //somewhere appropriate
  fooDrawInstance->SetFlags( b2Draw::e_shapeBit );
  debug_render_window = &window;
	layerZero = new MapLayer(map, 0);
	bg = new MapLayer(map, 1);
	playerTop = new Player(world);
	//playerBottom = new Player(world);
	myView.setSize(1707,1280);
	
	for (auto& layer:map.getLayers()) {
		if (map.getOrientation() == tmx::Orientation::Orthogonal &&
			layer->getType() == tmx::Layer::Type::Object && layer->getName() == "Collision") {
			auto collision_layer = *dynamic_cast<const tmx::ObjectGroup*>(layer.get());
			for (auto& obj: collision_layer.getObjects()) {
				// create the b2shape with a fixture
				b2PolygonShape b2shape = createShape(obj);
				b2FixtureDef fixtureDef;
				fixtureDef.density = 1.f;
				fixtureDef.friction = 0.7f;
				fixtureDef.shape = &b2shape;
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

Level::~Level() {
	delete layerZero;
	delete bg;
	//delete playerBottom;
	delete playerTop;
}

void Level::processEvent(sf::Event& event) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.type == sf::Event::JoystickButtonPressed) {
		bool toggleSwitch = false;
		unsigned int joystickId = event.joystickButton.joystickId;
		unsigned int joystickButton = event.joystickButton.button;
		std::cout<<"joystick id: "<<joystickId<<", button: "<<joystickButton<<std::endl;
		//if (sf::Joystick::isButtonPressed(joystickId, 5)) // RB button
		if (sf::Joystick::getAxisPosition(joystickId, sf::Joystick::R)>60.) {
			toggleSwitch = true;
		}
		if (joystickId == 0) {
			if (toggleSwitch) {
				playerTop->ActionSwap(PlayerState::NONE);
			} else {
				playerTop->ActionTrigger(PlayerState::JUMPING);
			}
		}
	}
	
	if (event.type == sf::Event::JoystickMoved) {
		if (event.joystickMove.axis == sf::Joystick::Axis::R) {
			std::cout << "axis moved: " <<event.joystickMove.axis<< std::endl;
			std::cout << "joystick id: " << event.joystickMove.joystickId << std::endl;
			std::cout << "new position: " << event.joystickMove.position << std::endl;
		}
	}
}

void Level::logic(const sf::Time deltaT) {
	world.Step(deltaT.asSeconds(), 8, 3);
	auto scale = Settings::instance()->getProperty<float>("box2d_scale");
	auto level_speed = Settings::instance()->getProperty<float>("level_speed");
	myView.setCenter(myView.getCenter().x + level_speed * deltaT.asSeconds() / scale, 1280./2);
	playerTop->update(deltaT.asSeconds());
	//playerBottom->update(deltaT.asSeconds());
}

void Level::draw() {
	window.setView(myView);
	window.draw(*layerZero);
	window.draw(*bg);
	window.draw(*playerTop);
	world.DrawDebugData();
	//window.draw(*playerBottom);
}

b2PolygonShape Level::createShape(const tmx::Object& obj) {
	switch(obj.getShape()) {
		case tmx::Object::Shape::Rectangle: {
			auto aabb = obj.getAABB();
			auto scale = Settings::instance()->getProperty<float>("box2d_scale");
			b2PolygonShape b2shape;
			b2shape.SetAsBox(
				(aabb.width * 0.5) * scale,
				(aabb.height * 0.5) * scale,
				b2Vec2(
					(aabb.left + aabb.width * 0.5) * scale,
					(aabb.top + aabb.height * 0.5) * scale
				), 0
			);
			return b2shape;
		}
		case tmx::Object::Shape::Polygon: {
			// deactivate erroneous code
			/*auto points = obj.getPoints();
			auto pos = obj.getPosition();
			b2PolygonShape b2shape;
			std::vector<b2Vec2> b2points;
			b2points.reserve(points.size());
			for (auto& point: points) {
				b2points.push_back(b2Vec2(pos.x + point.x, pos.y + point.y));
			}
			b2shape.Set(b2points.data(), b2points.size());
			return b2shape;*/
			break;
		}
		default: {
			break;
			// TODO implement tmx::Object::Shape::Ellipse and tmx::Object::Shape::Polyline
		}
		
	}
	return b2PolygonShape();
}
