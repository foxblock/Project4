#include "ProjectileLaser.h"

ProjectileLaser::ProjectileLaser( StateLevel *newParent, const int &duration ) :
	UnitBase( newParent, &shape )
{
	x = &(shape.target.x);
	y = &(shape.target.y);
	props.addFlag(ufDeadlyOnTouch);
	props.addFlag(ufInvincible);
	life.start( duration );
	timers.push_back( &life );
}

ProjectileLaser::~ProjectileLaser()
{

}


///--- PUBLIC ------------------------------------------------------------------

int ProjectileLaser::update( Uint32 delta )
{
	if ( life.isStopped() )
		toBeRemoved = true;
	return UnitBase::update( delta );
}

void ProjectileLaser::render( SDL_Surface *target )
{
	spLine( shape.pos.x, shape.pos.y, -1, shape.target.x, shape.target.y, -1,
			spGetRGB( 255, 0, 0 ) );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
