#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
  #include <iostream>
  
  static sf::RenderWindow *debug_render_window = 0;
void drawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	float scaleFactor = 30.;
	sf::ConvexShape convex;
	
	convex.setPointCount(vertexCount);
	
	for (int i = 0; i < vertexCount; i++)
	{
		convex.setPoint(i,sf::Vector2f(vertices[i].x*scaleFactor,vertices[i].y*scaleFactor));
	}
	convex.setFillColor(sf::Color(100, 250, 50,50));
	std::cout<<"HILSUEFLISUEFB"<<std::endl;
	debug_render_window->draw(convex);
}


  
  class FooDraw : public b2Draw
  {
  public:
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {drawPolygon(vertices, vertexCount, color);}
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {drawPolygon(vertices, vertexCount, color);}
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
    void DrawTransform(const b2Transform& xf) {}
  };
