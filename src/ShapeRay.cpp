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
				(other->infiniteLength || m1 <= other->length()))
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
				(infiniteLength || m1 <= length()))
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
	if ( infiniteLength || ( m1 >= 0 && m1 <= length() ) )
	{
		if ( other->infiniteLength || ( m2 >= 0 && m2 <= other->length() ) )
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
	Vector2d<float> dir = (target - pos).unit();
	Vector2d<float> rectTL = other->getCorner(diTOPLEFT);
	Vector2d<float> rectBR = other->getCorner(diBOTTOMRIGHT);

	// Determine quadrant the ray start and end is in
	Vector2d<int> quadPos((pos.x > rectBR.x) - (pos.x < rectTL.x),
			(pos.y > rectBR.y) - (pos.y < rectTL.y));
	Vector2d<int> quadTarget((target.x > rectBR.x) - (target.x < rectTL.x),
			(target.y > rectBR.y) - (target.y < rectTL.y));

	// No collision if ray fully is in one quadrant outside rect
	if (quadPos == quadTarget && quadPos.x != 0 && quadPos.y != 0 && !infiniteLength)
		return false;

	// No collision if ray is on one side of rect (=misses it)
	if ((quadPos.x == quadTarget.x && quadPos.x != 0) ||
			(quadPos.y == quadTarget.y && quadTarget.y != 0))
		return false;

	// Do simplified ray-ray collision with the closest side(s) of the rect to the starting point of the ray
	// (Depending on the quadrant the ray can only hit one or one of two sides of the rect):
	// Calculate m factor for x (or y) position
	// check if corresponding y (or x) position is on the rect
	// and check whether it is on the ray
	// TODO: Could probably make this code a lot more compact...
	if (quadPos.x == -1)
	{
		// Possible collision point with left side
		float m1 = (rectTL.x - pos.x) / dir.x;
		if (quadPos.y == -1)
		{
			// Possible collision point with top side
			float m2 = (rectTL.y - pos.y) / dir.y;
			// True collision always happens on the side with the bigger m
			if (m1 > m2)
			{
				// Left side
				// Because of the "bigger m"-assumption we only need to check for far side
				float posY = pos.y + dir.y * m1;
				if (posY <= rectBR.y && (infiniteLength || m1 <= length()))
				{
					result.colliding = true;
					result.position.x = pos.x + m1 * dir.x;
					result.position.y = posY;
					result.direction.x = -1;
					result.lhs = true;
					return true;
				}
			}
			else
			{
				// Top side
				float posX = pos.x + dir.x * m2;
				if (posX <= rectBR.x && (infiniteLength || m2 <= length()))
				{
					result.colliding = true;
					result.position.x = posX;
					result.position.y = pos.y + dir.y * m2;
					result.direction.y = -1;
					return true;
				}
			}
		}
		else if (quadPos.y == 0)
		{
			// Can only collide with left side
			float posY = pos.y + m1 * dir.y;
			if (posY >= rectTL.y && posY <= rectBR.y && (infiniteLength || m1 <= length()))
			{
				result.colliding = true;
				result.position.x = pos.x + m1 * dir.x;
				result.position.y = posY;
				result.direction.x = -1;
				result.lhs = posY > pos.y;
				return true;
			}
		}
		else // quadPos.y == 1
		{
			// Possible collision point with bottom side
			float m2 = (rectBR.y - pos.y) / dir.y;
			if (m1 > m2)
			{
				float posY = pos.y + dir.y * m1;
				if (posY >= rectTL.y && (infiniteLength || m1 <= length()))
				{
					result.colliding = true;
					result.position.x = pos.x + m1 * dir.x;
					result.position.y = posY;
					result.direction.x = -1;
					return true;
				}
			}
			else
			{
				// Bottom side
				float posX = pos.x + dir.x * m2;
				if (posX <= rectBR.x && (infiniteLength || m2 <= length()))
				{
					result.colliding = true;
					result.position.x = posX;
					result.position.y = pos.y + dir.y * m2;
					result.direction.y = 1;
					result.lhs = true;
					return true;
				}
			}
		}
	}
	else if (quadPos.x == 0)
	{
		if (quadPos.y == -1)
		{
			float m = (rectTL.y - pos.y) / dir.y;
			float posX = pos.x + dir.x * m;
			if (posX >= rectTL.x && posX <= rectBR.x && (infiniteLength || m < length()))
			{
                result.colliding = true;
                result.position.x = posX;
                result.position.y = pos.y + dir.y * m;
                result.direction.y = -1;
				result.lhs = posX < pos.x;
                return true;
			}
		}
		if (quadPos.y == 0)
		{
			// Ray is inside of rect
			// TODO: This (check sides depending on target, basically the same code, just with quadPos and quadTarget swapped, also result.direction and always go for smaller m)
		}
		else // quadPos.y == 1
		{
			float m = (rectBR.y - pos.y) / dir.y;
			float posX = pos.x + dir.x * m;
			if (posX >= rectTL.x && posX <= rectBR.x && (infiniteLength || m < length()))
			{
				result.colliding = true;
				result.position.x = posX;
				result.position.y = pos.y + dir.y * m;
				result.direction.y = 1;
				result.lhs = posX > pos.x;
				return true;
			}
		}
	}
	else // quadPos.x == 1
	{
		float m1 = (rectBR.x - pos.x) / dir.x;
		if (quadPos.y == -1)
		{
			float m2 = (rectTL.y - pos.y) / dir.y;
			if (m1 > m2)
			{
				float posY = pos.y + dir.y * m1;
				if (posY <= rectBR.y && (infiniteLength || m1 <= length()))
				{
					result.colliding = true;
					result.position.x = pos.x + m1 * dir.x;
					result.position.y = posY;
					result.direction.x = 1;
					return true;
				}
			}
			else
			{
				float posX = pos.x + dir.x * m2;
				if (posX >= rectTL.x && (infiniteLength || m2 <= length()))
				{
					result.colliding = true;
					result.position.x = posX;
					result.position.y = pos.y + dir.y * m2;
					result.direction.y = -1;
					result.lhs = true;
					return true;
				}
			}
		}
		else if (quadPos.y == 0)
		{
			float posY = pos.y + m1 * dir.y;
			if (posY >= rectTL.y && posY <= rectBR.y && (infiniteLength || m1 <= length()))
			{
				result.colliding = true;
				result.position.x = pos.x + m1 * dir.x;
				result.position.y = posY;
				result.direction.x = 1;
				result.lhs = posY < pos.y;
				return true;
			}
		}
		else // quadPos.y == 1
		{
			float m2 = (rectBR.y - pos.y) / dir.y;
			if (m1 > m2)
			{
				float posY = pos.y + dir.y * m1;
				if (posY >= rectTL.y && (infiniteLength || m1 <= length()))
				{
					result.colliding = true;
					result.position.x = pos.x + m1 * dir.x;
					result.position.y = posY;
					result.direction.x = 1;
					result.lhs = true;
					return true;
				}
			}
			else
			{
				float posX = pos.x + dir.x * m2;
				if (posX >= rectTL.x && (infiniteLength || m2 <= length()))
				{
					result.colliding = true;
					result.position.x = posX;
					result.position.y = pos.y + dir.y * m2;
					result.direction.y = 1;
					return true;
				}
			}
		}
	}
	return false;
}

bool ShapeRay::checkCollision(ShapeRay const* const other, CollisionResponse& result) const
{
	Vector2d<float> dir = ( target - pos ).unit();
	Vector2d<float> otherDir = ( other->target - other->pos ).unit();

	// Corner cases (where one or both rays are 1D points so the dir vector is 0-length)
	if ((dir.x == 0 && dir.y == 0) && (otherDir.x == 0 && otherDir.y == 0))
	{
		// if both are points, just check for quality
		if (fabs(pos.x - other->pos.x) < FLOAT_ACCURACY && fabs(pos.y - other->pos.y) < FLOAT_ACCURACY)
		{
			result.colliding = true;
			result.position = pos;
		}
		return result.colliding;
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
				(other->infiniteLength || m1 <= other->length()))
		{
			result.colliding = true;
			result.position = pos;
		}
		return result.colliding;
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
				(infiniteLength || m1 <= length()))
		{
			result.colliding = true;
			result.position = other->pos;
		}
		return result.colliding;
	}

	// Check whether rays are parallel (never collide)
	if (fabs(dir.x - otherDir.x) < FLOAT_ACCURACY && fabs(dir.y - otherDir.y) < FLOAT_ACCURACY)
		return false;

	// Note: Can't do a quick exit here if both are infinite length, because we need to calc the exact point of the collision
//	if ( infiniteLength && other->infiniteLength )
//		return true;

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
	if ( infiniteLength || ( m1 >= 0 && m1 <= length() ) )
	{
		if ( other->infiniteLength || ( m2 >= 0 && m2 <= other->length() ) )
		{
			result.colliding = true;
			result.position = pos + dir * m1;
			return true;
		}
	}

	return false;
}

bool ShapeRay::checkCollision(ShapeCircle const* const other, CollisionResponse& result) const
{
	float angle = (target - pos).angle();
	// Rotate all positions to a a coordinate space aligned to the ray (x-axis parallel to ray)
	Vector2d<float> circlePos = other->pos.rotate(angle);
	Vector2d<float> rayPos = pos.rotate(angle);
	Vector2d<float> rayTarget = target.rotate(angle);
	float rayLeftPos = std::min(rayPos.x, rayTarget.x);
	float rayRightPos = std::max(rayPos.x, rayTarget.x);

	// Now checking whether the circle overlaps is simple
	if (circlePos.y < rayPos.y - other->radius || circlePos.y > rayPos.y + other->radius)
		return false;
	if (circlePos.x < rayLeftPos - other->radius || circlePos.x > rayRightPos + other->radius)
		return false;
	if ((circlePos.x < rayLeftPos && (circlePos - Vector2d<float>(rayLeftPos, rayPos.y)).length() > other->radius) ||
			(circlePos.x > rayRightPos && (circlePos - Vector2d<float>(rayRightPos, rayPos.y)).length() > other->radius))
		return false;

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

	return true;
}

void ShapeRay::render( SDL_Surface *target, Uint32 colour )
{
	spLine( pos.x, pos.y, -1, this->target.x, this->target.y, -1, colour );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

