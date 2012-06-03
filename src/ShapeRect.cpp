#include "ShapeRect.h"

#include "ShapeRay.h"

ShapeRect::ShapeRect() :
	ShapeBase(),
	size( 0, 0 )
{
	//
}

ShapeRect::~ShapeRect()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

bool ShapeRect::checkCollision( ShapeBase const *const other) const
{
	return other->checkCollision( this );
}

bool ShapeRect::checkCollision( ShapeRect const *const other ) const
{
	if ( ( ( pos.x - size.x / 2.0f < other->pos.x + other->size.x / 2.0f ) &&
			( other->pos.x - other->size.x / 2.0f < pos.x + size.x / 2.0f ) ) &&
			( ( pos.y - size.y / 2.0f < other->pos.y + other->size.y / 2.0f ) &&
			  ( other->pos.y - other->size.y / 2.0f < pos.y + size.y / 2.0f ) ) )
		return true;
	return false;
}

bool ShapeRect::checkCollision( ShapeRay const *const other ) const
{
	return other->checkCollision( this );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
