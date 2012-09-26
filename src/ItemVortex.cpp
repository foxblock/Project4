#include "ItemVortex.h"

#include "Events.h"
#include "gameDefines.h"
#include "StateLevel.h"

#define ITEM_VORTEX_RADIUS 20
#define ITEM_VORTEX_LIFE 5000
#define ITEM_VORTEX_DURATION 2000
#define ITEM_VORTEX_SIZETIME 40
#define ITEM_VORTEX_SIZERATE 2

SDL_Surface* ItemVortex::idle = NULL;

ItemVortex::ItemVortex( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	activeSprite = idle;

	shape.radius = ITEM_VORTEX_RADIUS;
	x = &( shape.pos.x );
	y = &( shape.pos.y );

	life.start( ITEM_VORTEX_LIFE );
	timers.push_back( &vortex );
	timers.push_back( &life );
}

ItemVortex::~ItemVortex()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int ItemVortex::update( const Uint32 &delta )
{
	UnitBase::update( delta );

	if ( life.isStopped() && (vortex.isStopped() || !vortex.wasStarted() ) )
	{
		toBeRemoved = true;
	}
	if ( vortex.wasStarted() && !vortex.isStopped() )
	{
		if( vortex.getTime() <= ITEM_VORTEX_SIZETIME )
			shape.radius += ITEM_VORTEX_SIZERATE;
		if( vortex.getTime() >= ITEM_VORTEX_DURATION - ITEM_VORTEX_SIZETIME )
			shape.radius += ITEM_VORTEX_SIZERATE;
	}
}

void ItemVortex::render( SDL_Surface *const target )
{
	if ( vortex.wasStarted() && !vortex.isStopped() )
	{
		spEllipse( *x, *y, -1, shape.radius, shape.radius, spGetFastRGB( 192, 192, 192 ) );
	}
	UnitBase::render( target );
}

void ItemVortex::ai(const Uint32& delta, UnitBase* const player )
{
	// All AI code goes here (reaction to the player except collision)
	if ( vortex.wasStarted() && !vortex.isStopped() )
	{
		// TODO: Hold enemies within shape radius
	}

	// This makes the item reverse direction when touching a wall (items might move at a later point)
	if ( (*x < shape.radius && accel.x < 0) ||
			(*x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
		accel.x *= -1;
	if ( ( *y < shape.radius && accel.y < 0 ) ||
			(*y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
		accel.y *= -1;
}

void ItemVortex::collisionResponse( UnitBase* const other )
{
	UnitBase::collisionResponse( other );
	if ( other->flags.has(ufIsPlayer) )
	{
		// This code is executed when the player collects the item
		// So your event code should go here (or be called from here)

		life.stop();
		shape.radius = 0;
		vortex.start( ITEM_VORTEX_DURATION );
		flags.add( UnitBase::ufInvincible );
		// toBeRemoved = true; // removes the unit/item from the game
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

void ItemVortex::generateIdleImage()
{
	idle = spCreateSurface( ITEM_VORTEX_RADIUS * 2, ITEM_VORTEX_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	Uint16 col = spGetRGB( 0, 192, 0 );
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1);
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.9, ITEM_VORTEX_RADIUS * 0.9, col );
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.65, ITEM_VORTEX_RADIUS * 0.65, -1);
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.55, ITEM_VORTEX_RADIUS * 0.55, col );
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.45, ITEM_VORTEX_RADIUS * 0.45, -1);
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.35, ITEM_VORTEX_RADIUS * 0.35, col );
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.25, ITEM_VORTEX_RADIUS * 0.25, -1);
	spEllipse( ITEM_VORTEX_RADIUS, ITEM_VORTEX_RADIUS, -1, ITEM_VORTEX_RADIUS * 0.15, ITEM_VORTEX_RADIUS * 0.15, col );
	spSelectRenderTarget( spGetWindowSurface() );
}

