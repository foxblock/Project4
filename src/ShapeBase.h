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
		,direction(0.0f, 0.0f)
		,position(0.0f, 0.0f)
		,distance(0.0f)
		,lhs(false)
	{}
	bool colliding;
	// Direction vector is aimed from other shape to this (=direction in which this shape has to move to solve collision)
	// Length of direction vector should be 1
	// For any collision with a ray and another shape that is not a ray, this is the normal vector to the collision surface
	// For two rays this is a 0-vector
	Vector2d<float> direction;
	// Position of the collision point for ray collisions (with any other object)
	// For a collision where no ray shape is involved (i.e. two circles) this is a 0-vector
	Vector2d<float> position;
	// Amount of overlap (both units have to move this much in sum to fully solve collition.
	// This amount will be split in UnitBase::checkCollision based on mass and velocity between the two units)
	// Always 0 when one or both of the colliding objects is a ray
	float distance;
	// For Ray-Circle or Ray-Rect collisions.
	// Set to true if the circle or rect is on the left side of ray, false otherwise
	// Defined as looking from the pos of the ray to the target
	// Used to determine in what direction to reflect
	bool lhs;
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

