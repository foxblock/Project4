#include "ShapeCircle.h"

#include <cmath>
#include "ShapeRect.h"
#include "ShapeRay.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"
#include SPARROW_PATH

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
	if ( Utility::sqr( other->pos.x - pos.x ) + Utility::sqr( other->pos.y - pos.y ) <
			Utility::sqr( radius + other->radius ) )
		return true;
	return false;
}

void ShapeCircle::render( SDL_Surface *target, Uint32 colour )
{
	spEllipseBorder( pos.x, pos.y, -1, radius, radius, 1, 1, colour );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
