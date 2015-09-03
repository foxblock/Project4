#include "ShapeCircle.h"

#include "ShapeRect.h"
#include "ShapeRay.h"
#include "UtilityFunctions.h"

#include <cmath>
#include "sparrowPrimitives.h"

ShapeCircle::ShapeCircle() :
	ShapeBase(),
	radius( 0.0f )
{
	//
}

ShapeCircle::~ShapeCircle()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

bool ShapeCircle::checkCollision( ShapeBase const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeCircle::checkCollision( ShapeRect const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeCircle::checkCollision( ShapeRay const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeCircle::checkCollision( ShapeCircle const *const other ) const
{
	if ( Utility::sqr<float>( other->pos.x - pos.x ) + Utility::sqr<float>( other->pos.y - pos.y ) <
			Utility::sqr<float>( radius + other->radius ) )
		return true;
	return false;
}

void ShapeCircle::render( SDL_Surface *target, Uint32 colour )
{
	spEllipseBorder( pos.x, pos.y, -1, radius, radius, 1, 1, colour );
}

bool ShapeCircle::checkCollision( ShapeBase const *const other, CollisionResponse &result ) const
{
	bool temp = other->checkCollision( this, result );
	result.direction *= -1;
	return temp;
}

bool ShapeCircle::checkCollision( ShapeRect const *const other, CollisionResponse &result ) const
{
	bool temp = other->checkCollision( this, result );
	result.direction *= -1;
	return temp;
}

bool ShapeCircle::checkCollision( ShapeRay const *const other, CollisionResponse &result ) const
{
	bool temp = other->checkCollision( this, result );
	result.direction *= -1;
	return temp;
}

bool ShapeCircle::checkCollision( ShapeCircle const *const other, CollisionResponse &result ) const
{
	Vector2d<float> distanceVec = pos - other->pos;
	float distance = distanceVec.length();
	float temp = radius + other->radius - distance;
	if ( temp > 0.0f )
	{
		result.colliding = true;
		result.direction = distanceVec.unit();
		result.distance = temp;
	}
	else
	{
		result.colliding = false;
		result.direction = Vector2d<float>( 0.0f, 0.0f );
		result.distance = 0.0f;
	}
	return result.colliding;
}


///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
