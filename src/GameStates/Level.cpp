#include "Level.hpp"

#include <tmxlite/ObjectGroup.hpp>

#include "../Settings.hpp"

#include <iostream>
#include <memory>

#include "Box2DDebugDrawer.hpp"

Level::Level(sf::RenderWindow& window):
	GameState(window),
	useKeyboard(!sf::Joystick::isConnected(1)),
	gravity(b2Vec2(0.f, Settings::instance()->getProperty<float>("gravity"))),
	world(b2World(gravity)),
	map()
{
	lua_State *state = luaL_newstate();
	luaL_loadfile(state, "assets/scripts/pcg.lua");
	lua_pcall(state, 0, LUA_MULTRET, 0);

	map.load(Settings::instance()->getProperty<std::string>("level_file"));
	isImmortal = Settings::instance()->getProperty<int>("immortal");
	//at global scope
	b2Draw *fooDrawInstance = new FooDraw();

	//in constructor, usually
	world.SetDebugDraw( fooDrawInstance );

	//somewhere appropriate
	fooDrawInstance->SetFlags( b2Draw::e_shapeBit );
	debug_render_window = &window;
	layerZero = new MapLayer(map, 0);
	bg = new MapLayer(map, 1);
	playerTop = new Player(world, false, {100,100});
	playerTop->ActionSwap(PlayerState::JUMPING);
	playerTop->ActionSwap(PlayerState::FLYING);
	playerTop->ActionSwap(PlayerState::SLIDING);
	skillJump = sf::CircleShape(16);
	skillJump.setPosition({100, 100});
	skillJump.setFillColor({0, 255, 0, 100});
	skillJump.setOutlineColor({0,0,0,255});
	skillJump.setOutlineThickness(4);
	skillFly = sf::CircleShape(16);
	skillFly.setPosition({100, 50});
	skillFly.setFillColor({255, 0, 0, 100});
	skillFly.setOutlineColor({0,0,0,255});
	skillFly.setOutlineThickness(4);
	skillSlide = sf::CircleShape(16);
	skillSlide.setPosition({100, 50});
	skillSlide.setFillColor({0, 0, 255, 100});
	skillSlide.setOutlineColor({0,0,0,255});
	skillSlide.setOutlineThickness(4);
	playerBottom = new Player(world, true, {100,800});
	myView.setSize(
		Settings::instance()->getProperty<float>("view_width"),
		Settings::instance()->getProperty<float>("view_height")
	);
	
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
	delete playerBottom;
	delete playerTop;
}

static std::map<unsigned int, PlayerState> buttonLayout = {{0u,PlayerState::JUMPING},{1u,PlayerState::FLYING},{2u,PlayerState::SLIDING}};

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
			if (toggleSwitch && playerTop->hasSkill(buttonLayout[joystickButton])) {
				playerTop->ActionSwap(buttonLayout[joystickButton]);
				playerBottom->ActionSwap(buttonLayout[joystickButton]);
			} else {
				playerTop->ActionTrigger(buttonLayout[joystickButton]);
			}
		}
		if (joystickId == 1) {
			if (toggleSwitch && playerBottom->hasSkill(buttonLayout[joystickButton])) {
				playerTop->ActionSwap(buttonLayout[joystickButton]);
				playerBottom->ActionSwap(buttonLayout[joystickButton]);
			} else {
				playerBottom->ActionTrigger(buttonLayout[joystickButton]);
			}
		}
	}
	
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::F3)
		{
			isImmortal = 3;
		}
		if (event.key.code == sf::Keyboard::F1)
		{
			isImmortal = 1;
		}
		if (event.key.code == sf::Keyboard::F2)
		{
			isImmortal = 2;
		}
		if (useKeyboard) {
			bool toggleSwitch = event.key.shift;
			if (event.key.code == sf::Keyboard::S) {
				if (toggleSwitch) {
					playerTop->ActionSwap(PlayerState::JUMPING);
					playerBottom->ActionSwap(PlayerState::JUMPING);
				} else {
					playerBottom->ActionTrigger(PlayerState::JUMPING);
				}
			}
			if (event.key.code == sf::Keyboard::K) {
				if (toggleSwitch) {
					playerTop->ActionSwap(PlayerState::JUMPING);
					playerBottom->ActionSwap(PlayerState::JUMPING);
				} else {
					playerTop->ActionTrigger(PlayerState::JUMPING);
				}
			}
			if (event.key.code == sf::Keyboard::W) {
				if (toggleSwitch) {
					playerTop->ActionSwap(PlayerState::FLYING);
					playerBottom->ActionSwap(PlayerState::FLYING);
				} else {
					playerBottom->ActionTrigger(PlayerState::FLYING);
				}
			}
			if (event.key.code == sf::Keyboard::I) {
				if (toggleSwitch) {
					playerTop->ActionSwap(PlayerState::FLYING);
					playerBottom->ActionSwap(PlayerState::FLYING);
				} else {
					playerTop->ActionTrigger(PlayerState::FLYING);
				}
			}
		}
		if (event.key.code == sf::Keyboard::Escape) {
			window.close();
		}
		if (event.key.code == sf::Keyboard::R) {
			nextState = std::unique_ptr<GameState>(new Level(window));
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
	myView.setCenter(
		myView.getCenter().x + level_speed * deltaT.asSeconds() / scale,
		Settings::instance()->getProperty<float>("view_height") / 2
	);
	playerTop->update(deltaT.asSeconds());
	playerBottom->update(deltaT.asSeconds());

	// die
	if ((playerTop->hasContact().second && (isImmortal == 0 || isImmortal == 2)) || (playerBottom->hasContact().second && (isImmortal == 0 || isImmortal ==1))) {
		nextState = std::unique_ptr<GameState>(new Deathscreen(window));
	}

	// skill following the correct player
	float diffTop = -40;
	float diffBottom = -40;
	if (playerTop->hasSkill(PlayerState::JUMPING)) {
		//std::cout<<playerTop->getPos().x << std::endl;
		skillJump.setPosition(playerTop->getPos() + sf::Vector2f(playerTop->getSize().x / 2 + diffTop, 0));
		diffTop += 40;
	} else if (playerBottom->hasSkill(PlayerState::JUMPING)) {
		skillJump.setPosition(playerBottom->getPos() + sf::Vector2f(playerBottom->getSize().x / 2  + diffBottom, 0));
		diffBottom += 40;
	}
	if (playerTop->hasSkill(PlayerState::FLYING)) {
		skillFly.setPosition(playerTop->getPos() + sf::Vector2f(playerTop->getSize().x / 2  + diffTop, 0));
		diffTop += 40;
	} else if (playerBottom->hasSkill(PlayerState::FLYING)) {
		skillFly.setPosition(playerBottom->getPos() + sf::Vector2f(playerBottom->getSize().x / 2 + diffBottom, 0));
		diffBottom += 40;
	}
	if (playerTop->hasSkill(PlayerState::SLIDING)) {
		skillSlide.setPosition(playerTop->getPos() + sf::Vector2f(playerTop->getSize().x / 2  + diffTop, 0));
		diffTop += 40;
	} else if (playerBottom->hasSkill(PlayerState::SLIDING)) {
		skillSlide.setPosition(playerBottom->getPos() + sf::Vector2f(playerBottom->getSize().x / 2 + diffBottom, 0));
		diffBottom += 40;
	}
}

void Level::draw() {
	window.setView(myView);
	window.draw(*layerZero);
	window.draw(*bg);
	window.draw(*playerTop);
	world.DrawDebugData();
	window.draw(*playerBottom);
	window.draw(skillJump);
	window.draw(skillFly);
	window.draw(skillSlide);
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
