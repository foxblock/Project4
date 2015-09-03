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
	// Direction vector is aimed from other shape to this (=direction in which this shape has to move to solve collision)
	// Length of direction vector should be 1
	Vector2d<float> direction;
	// Amount of overlap (both units have to move this much in sum to fully solve collition.
	// This amount will be split in UnitBase::checkCollision based on mass and velocity between the two units)
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

