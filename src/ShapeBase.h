#ifndef _SHAPE_BASE_H
#define _SHAPE_BASE_H

#include "Vector2d.h"
#include <cstdio>
#include "SDL.h"

class ShapeRect;
class ShapeRay;
class ShapeCircle;

struct CollisionResponse
{
	CollisionResponse()
		:colliding(false)
		,direction(0.0f,0.0f)
		,distance(0.0f)
	{}
	bool colliding;
	Vector2d<float> direction;
	float distance;
};

class ShapeBase
{
public:
	ShapeBase()
		:pos(0.0f,0.0f)
		{ };
	virtual ~ShapeBase() { };

	virtual bool checkCollision( ShapeBase const * const other ) const = 0;
	virtual bool checkCollision( ShapeRect const * const other ) const = 0;
	virtual bool checkCollision( ShapeRay const * const other ) const = 0;
	virtual bool checkCollision( ShapeCircle const * const other ) const = 0;

	virtual bool checkCollision( ShapeBase const * const other, CollisionResponse &result ) const = 0;
	virtual bool checkCollision( ShapeRect const * const other, CollisionResponse &result ) const = 0;
	virtual bool checkCollision( ShapeRay const * const other, CollisionResponse &result ) const = 0;
	virtual bool checkCollision( ShapeCircle const * const other, CollisionResponse &result ) const = 0;

	virtual void render( SDL_Surface* target, Uint32 colour ) = 0;

	Vector2d<float> pos;
	CollisionResponse response;
protected:

private:

};


#endif // _SHAPE_BASE_H

