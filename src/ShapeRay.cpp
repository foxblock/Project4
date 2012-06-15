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

bool ShapeRay::checkCollision( ShapeRay const *const other ) const
{
	Vector2d<float> dir = ( target - pos ).unit();
	Vector2d<float> otherDir = ( other->target - other->pos ).unit();

	if ( fabs( dir.x - otherDir.x - dir.y + otherDir.y ) < FLOAT_ACCURACY )
		return false;

	if ( infiniteLength && other->infiniteLength )
		return true;

	float m2 = ( ( other->pos.y - pos.y ) * dir.x - ( other->pos.x - pos.x ) * dir.y ) /
			   ( otherDir.x * dir.y - otherDir.y * dir.x );
	float m1 = ( ( other->pos.x - pos.x ) + m2 * otherDir.x ) / dir.x;

	if ( infiniteLength || ( m1 >= 0 && fabs( m1 ) * dir.length() < length() ) )
	{
		if ( other->infiniteLength || ( m2 >= 0 && fabs( m2 ) * otherDir.length() < other->length() ) )
			return true;
	}

	return false;
}

bool ShapeRay::checkCollision( ShapeCircle const *const other ) const
{
	ShapeRay temp;
	temp.pos = other->pos;
	Vector2d<float> dir = ( target - pos ).unit();
	dir = Vector2d<float>( -dir.y, dir.x );
	temp.target = temp.pos + dir * other->radius;
	if ( checkCollision( &temp ) )
		return true;
	else
	{
		temp.target = temp.pos - dir * other->radius;
		return checkCollision( &temp );
	}
}

void ShapeRay::render( SDL_Surface *target, Uint32 colour )
{
	spLine( pos.x, pos.y, -1, this->target.x, this->target.y, -1, colour );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

