#include "ProjectileLaser.h"

#include "sparrowPrimitives.h"

#define PROJECTILE_LASER_BLINK_TIME 75

ProjectileLaser::ProjectileLaser( StateLevel *newParent, const int &duration ) :
	UnitBase( newParent, &shape )
{
	x = &(shape.target.x);
	y = &(shape.target.y);
	flags.add(ufDeadlyOnTouch);
	flags.add(ufInvincible);
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

int ProjectileLaser::update( const Uint32 &delta )
{
	if ( life.stopped() )
		toBeRemoved = true;
	if ( blink.stopped() )
	{
		blinkStatus = !blinkStatus;
		blink.start( PROJECTILE_LASER_BLINK_TIME );
	}
	return UnitBase::update( delta );
}

void ProjectileLaser::render( SDL_Surface *const target )
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
