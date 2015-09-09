#include "ShapeRay.h"

#include "UtilityFunctions.h"
#include "ShapeRect.h"
#include "ShapeCircle.h"

#include <cmath>
#include "sparrowPrimitives.h"

ShapeRay::ShapeRay() :
	ShapeBase(),
	target( 0, 0 ),
	infiniteLength( false )
{
	//
}

ShapeRay::~ShapeRay()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

bool ShapeRay::checkCollision( ShapeBase const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeRay::checkCollision( ShapeRect const *const other ) const
{
	// TODO: Do this properly (not with ray approximation
	ShapeRay bltr;
	bltr.pos = other->getCorner( diBOTTOMLEFT );
	bltr.target = other->getCorner( diTOPRIGHT );
	ShapeRay brtl;
	brtl.pos = other->getCorner( diBOTTOMRIGHT );
	brtl.target = other->getCorner( diTOPLEFT );
	if ( checkCollision( &bltr ) || checkCollision( &brtl ) )
		return true;
	return false;
}

// These calculations solve the linear equation system
// (1) pos.x + dir.x * m1 == other->pos,x + m2 * otherDir.x
// (2) pos.y + dir.y * m1 == other->pos,y + m2 * otherDir.y
// Solve (1) for m1, insert into (2) and calculate m2
// Then insert into (1) or (2) to get m1
bool ShapeRay::checkCollision( ShapeRay const *const other ) const
{
	Vector2d<float> dir = ( target - pos ).unit();
	Vector2d<float> otherDir = ( other->target - other->pos ).unit();

	// Corner cases (where one or both rays are 1D points so the dir vector is 0-length)
	if ((dir.x == 0 && dir.y == 0) && (otherDir.x == 0 && otherDir.y == 0))
	{
		// if both are points, just check for quality
		return (fabs(pos.x - other->pos.x) < FLOAT_ACCURACY && fabs(pos.y - other->pos.y) < FLOAT_ACCURACY);
	}
	if (dir.x == 0 && dir.y == 0)
	{
		// if only one is, do simple ray - point collision check, similar to full ray-ray collision
		// solve same linear equations wither the lhs or rhs severly simplified due to the 0-vector
		float m1;
		if (otherDir.y != 0)
			m1 = (pos.y - other->pos.y) / otherDir.y;
		else
			m1 = (pos.x - other->pos.x) / otherDir.x;
		// check whether point is on ray
		if (fabs(other->pos.x + otherDir.x * m1 - pos.x) < FLOAT_ACCURACY &&
				fabs(other->pos.y + otherDir.y * m1 - pos.y) < FLOAT_ACCURACY &&
				(other->infiniteLength || m1 < other->length()))
			return true;
		return false;
	}
	else if (otherDir.x == 0 && otherDir.y == 0)
	{
		// same as above
		float m1;
		if (dir.y != 0)
			m1 = (other->pos.y - pos.y) / dir.y;
		else
			m1 = (other->pos.x - pos.x) / dir.x;
		if (fabs(pos.x + dir.x * m1 - other->pos.x) < FLOAT_ACCURACY &&
				fabs(pos.y + dir.y * m1 - other->pos.y) < FLOAT_ACCURACY &&
				(infiniteLength || m1 < length()))
			return true;
		return false;
	}

	// Check whether rays are parallel (never collide)
	if (fabs(dir.x - otherDir.x) < FLOAT_ACCURACY && fabs(dir.y - otherDir.y) < FLOAT_ACCURACY)
		return false;

	// Two infinite non-parallal rays will always collide at some point eventually
	if ( infiniteLength && other->infiniteLength )
		return true;

	// Now at this point there must be a collision if the rays were infinite
	// We have to calculate the point of the collision and check whether it is within bounds (length of rays)
	// Solve system of linear equations for collision point m1 on this ray and m2 on other ray
	float m2 = ( ( other->pos.y - pos.y ) * dir.x - ( other->pos.x - pos.x ) * dir.y ) /
			   ( otherDir.x * dir.y - otherDir.y * dir.x );
	float m1;
	if (dir.x != 0)
		m1 = ((other->pos.x - pos.x) + m2 * otherDir.x) / dir.x;
	else
		m1 = ((other->pos.y - pos.y) + m2 * otherDir.y) / dir.y;

	// Check whether collision points are on the actual rays
	if ( infiniteLength || ( m1 >= 0 && fabs( m1 ) < length() ) )
	{
		if ( other->infiniteLength || ( m2 >= 0 && fabs( m2 ) < other->length() ) )
			return true;
	}

	return false;
}

bool ShapeRay::checkCollision( ShapeCircle const *const other ) const
{
	float angle = (target - pos).angle();
	// Adjusted positions in other coordinate space
	Vector2d<float> circlePos = other->pos.rotate(angle);
	Vector2d<float> rayPos = pos.rotate(angle);
	Vector2d<float> rayTarget = target.rotate(angle);

	if (circlePos.x - other->radius <= rayTarget.x && circlePos.x + other->radius >= rayPos.x &&
			circlePos.y - other->radius <= rayPos.y && circlePos.y + other->radius >= rayPos.y)
	{
        return true;
	}
	return false;
}


bool ShapeRay::checkCollision(ShapeBase const* const other, CollisionResponse& result) const
{
	bool temp = other->checkCollision( this, result );
	result.direction *= -1;
	return temp;
}

bool ShapeRay::checkCollision(ShapeRect const* const other, CollisionResponse& result) const
{
	// TODO: Real collision detection with overlap!
	result.colliding = checkCollision( other );
	return result.colliding;
}

bool ShapeRay::checkCollision(ShapeRay const* const other, CollisionResponse& result) const
{
	// TODO: Real collision detection with overlap!
	result.colliding = checkCollision( other );
	return result.colliding;
}

bool ShapeRay::checkCollision(ShapeCircle const* const other, CollisionResponse& result) const
{
	float angle = (target - pos).angle();
	// Rotate all positions to a a coordinate space aligned to the ray (x-axis parallel to ray)
	Vector2d<float> circlePos = other->pos.rotate(angle);
	Vector2d<float> rayPos = pos.rotate(angle);
	Vector2d<float> rayTarget = target.rotate(angle);

	// Now checking whether the circle overlaps is simple
	// TODO: Not that simple though, dummy. This check assumes a square. Gotta check the distance to ray ends (if not infinite length)
	if (circlePos.x - other->radius <= std::max(rayTarget.x, rayPos.x) && circlePos.x + other->radius >= std::min(rayTarget.x, rayPos.x) &&
			circlePos.y - other->radius <= rayPos.y && circlePos.y + other->radius >= rayPos.y)
	{
        Vector2d<float> collisionPos(circlePos.x - sqrt(Utility::sqr(other->radius) - Utility::sqr(circlePos.y - rayPos.y)), rayPos.y);
        // Rotate output vectors back
        collisionPos.rotateThis(-angle);
        result.colliding = true;
        result.position = collisionPos;
        // Normal vector of circle surface at collision point
        result.direction = (collisionPos - other->pos).unit();
        // Is circle on the left hand side or right hand side of ray (looking from pos to target)
        // Used for reflection calculation
        if (circlePos.y < rayPos.y)
        	result.lhs = true;
       	else
       		result.lhs = false;
	}
	return result.colliding;
}

void ShapeRay::render( SDL_Surface *target, Uint32 colour )
{
	spLine( pos.x, pos.y, -1, this->target.x, this->target.y, -1, colour );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

