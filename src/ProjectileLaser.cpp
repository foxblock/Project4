#include "ProjectileLaser.h"

#define PROJECTILE_LASER_BLINK_TIME 100

ProjectileLaser::ProjectileLaser( StateLevel *newParent, const int &duration ) :
	UnitBase( newParent, &shape )
{
	x = &(shape.target.x);
	y = &(shape.target.y);
	props.addFlag(ufDeadlyOnTouch);
	props.addFlag(ufInvincible);
	life.start( duration );
	blink.start( PROJECTILE_LASER_BLINK_TIME );
	blinkStatus = 0;
	timers.push_back( &life );
	timers.push_back( &blink );
}

ProjectileLaser::~ProjectileLaser()
{

}


///--- PUBLIC ------------------------------------------------------------------

int ProjectileLaser::update( Uint32 delta )
{
	if ( life.isStopped() )
		toBeRemoved = true;
	if ( blink.isStopped() )
		blinkStatus = !blinkStatus;
	return UnitBase::update( delta );
}

void ProjectileLaser::render( SDL_Surface *target )
{
	if ( blinkStatus )
	{
		spLine( shape.pos.x, shape.pos.y, -1, shape.target.x, shape.target.y, -1,
				spGetFastRGB( 255, 0, 0 ) );
	}
	else
	{
		spLine( shape.pos.x, shape.pos.y, -1, shape.target.x, shape.target.y, -1,
				-1 );
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
