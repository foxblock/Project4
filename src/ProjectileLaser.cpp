#include "ProjectileLaser.h"

ProjectileLaser::ProjectileLaser( StateLevel *newParent, const int &duration ) :
	UnitBase( newParent, &shape )
{
	x = &(shape.target.x);
	y = &(shape.target.y);
	props.addFlag(ufDeadlyOnTouch);
	props.addFlag(ufInvincible);
	life.start( duration );
}

ProjectileLaser::~ProjectileLaser()
{

}


///--- PUBLIC ------------------------------------------------------------------

int ProjectileLaser::update( Uint32 delta )
{
	if (life.getStatus() == -1)
		toBeRemoved = true;
	return UnitBase::update( delta );
}

void ProjectileLaser::render( SDL_Surface *target )
{
	spLine( shape.pos.x, shape.pos.y, -1, shape.target.x, shape.target.y, -1,
			SDL_MapRGB( target->format, 255, 0, 0 ) );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
