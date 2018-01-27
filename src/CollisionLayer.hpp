#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Object.hpp>
#include <Box2D/Box2D.h>

#include <memory>
#include <vector>

class CollisionLayer {
public:
	CollisionLayer(const tmx::Map& map, b2World & world);
private:
	b2PolygonShape createShape(const tmx::Object& obj);
};
