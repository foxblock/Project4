#include "ItemSlowmo.h"

#include "Events.h"
#include "gameDefines.h"
#include "StateLevel.h"

#define ITEM_SLOWMO_RADIUS 20
#define ITEM_SLOWMO_LIFE 5000
#define ITEM_SLOWMO_DURATION 2000

SDL_Surface* ItemSlowmo::idle = NULL;

ItemSlowmo::ItemSlowmo( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	activeSprite = idle;

	shape.radius = ITEM_SLOWMO_RADIUS;
	x = &( shape.pos.x );
	y = &( shape.pos.y );

	life.start( ITEM_SLOWMO_LIFE );
	timers.push_back( &life );
}

ItemSlowmo::~ItemSlowmo()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int ItemSlowmo::update( const Uint32 &delta )
{
	UnitBase::update( delta );

	if ( life.isStopped() )
	{
		toBeRemoved = true;
	}
}

void ItemSlowmo::ai(const Uint32& delta, UnitBase* const)
{
	if ( (*x < shape.radius && accel.x < 0) ||
			(*x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
		accel.x *= -1;
	if ( ( *y < shape.radius && accel.y < 0 ) ||
			(*y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
		accel.y *= -1;
}

void ItemSlowmo::collisionResponse( UnitBase* const other )
{
	UnitBase::collisionResponse( other );
	if ( other->flags.has(ufIsPlayer) )
	{
		EventSlowMotion *event = new EventSlowMotion( ITEM_SLOWMO_DURATION );
		parent->addEvent( event );
		toBeRemoved = true;
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

void ItemSlowmo::generateIdleImage()
{
	idle = spCreateSurface( ITEM_SLOWMO_RADIUS * 2, ITEM_SLOWMO_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	spEllipse( ITEM_SLOWMO_RADIUS, ITEM_SLOWMO_RADIUS, -1, ITEM_SLOWMO_RADIUS, ITEM_SLOWMO_RADIUS, -1);
	spEllipse( ITEM_SLOWMO_RADIUS, ITEM_SLOWMO_RADIUS, -1, ITEM_SLOWMO_RADIUS-3, ITEM_SLOWMO_RADIUS-3,
			spGetFastRGB( 0, 255, 0 ) );
	spSelectRenderTarget( spGetWindowSurface() );
}

