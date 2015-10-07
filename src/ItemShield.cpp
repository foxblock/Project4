#include "ItemShield.h"

#include "gameDefines.h"
#include "UnitPlayer.h"

#include "sparrowPrimitives.h"

#define ITEM_SHIELD_RADIUS 16
#define ITEM_SHIELD_LIFE 5000
#define ITEM_SHIELD_DURATION 5000

SDL_Surface* ItemShield::idle = NULL;

ItemShield::ItemShield( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	activeSprite = idle;

	shape.radius = ITEM_SHIELD_RADIUS;
	x = &( shape.pos.x );
	y = &( shape.pos.y );

	flags.add( ufSolid );
	life.start( ITEM_SHIELD_LIFE );
	timers.push_back( &life );
}

ItemShield::~ItemShield()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int ItemShield::update( const Uint32 &delta )
{
	UnitBase::update( delta );

	if ( life.stopped() )
	{
		toBeRemoved = true;
	}
}

void ItemShield::ai( const Uint32& delta, UnitBase* const player )
{
	if ( (*x < shape.radius && accel.x < 0) ||
			(*x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
		accel.x *= -1;
	if ( ( *y < shape.radius && accel.y < 0 ) ||
			(*y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
		accel.y *= -1;
}

void ItemShield::collisionResponse( UnitBase* const other )
{
	UnitBase::collisionResponse( other );
	if ( other->flags.has(ufIsPlayer) )
	{
		((UnitPlayer*)other)->activeShield(ITEM_SHIELD_DURATION);
		toBeRemoved = true;
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

void ItemShield::generateIdleImage()
{
	idle = spCreateSurface( ITEM_SHIELD_RADIUS * 2, ITEM_SHIELD_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	Uint16 col = spGetRGB( 0, 192, 0 );
	spEllipse( ITEM_SHIELD_RADIUS, ITEM_SHIELD_RADIUS, -1, ITEM_SHIELD_RADIUS, ITEM_SHIELD_RADIUS, -1);
	spEllipse( ITEM_SHIELD_RADIUS, ITEM_SHIELD_RADIUS, -1, ITEM_SHIELD_RADIUS * 0.9, ITEM_SHIELD_RADIUS * 0.9, col );
	spSetLineWidth(1);
	spLine(ITEM_SHIELD_RADIUS * 0.5f, ITEM_SHIELD_RADIUS * 0.5f, -1, ITEM_SHIELD_RADIUS * 0.5f, ITEM_SHIELD_RADIUS * 1.2f, -1, -1);
	spLine(ITEM_SHIELD_RADIUS * 0.5f, ITEM_SHIELD_RADIUS * 1.2f, -1, ITEM_SHIELD_RADIUS * 1.0f, ITEM_SHIELD_RADIUS * 1.7f, -1, -1);
	spLine(ITEM_SHIELD_RADIUS * 1.0f, ITEM_SHIELD_RADIUS * 1.7f, -1, ITEM_SHIELD_RADIUS * 1.5f, ITEM_SHIELD_RADIUS * 1.2f, -1, -1);
	spLine(ITEM_SHIELD_RADIUS * 1.5f, ITEM_SHIELD_RADIUS * 1.2f, -1, ITEM_SHIELD_RADIUS * 1.5f, ITEM_SHIELD_RADIUS * 0.5f, -1, -1);
	spLine(ITEM_SHIELD_RADIUS * 1.5f, ITEM_SHIELD_RADIUS * 0.5f, -1, ITEM_SHIELD_RADIUS * 0.5f, ITEM_SHIELD_RADIUS * 0.5f, -1, -1);
	spLine(ITEM_SHIELD_RADIUS * 1.0f, ITEM_SHIELD_RADIUS * 0.5f, -1, ITEM_SHIELD_RADIUS * 1.0f, ITEM_SHIELD_RADIUS * 1.7f, -1, -1);
	spSetLineWidth(1);
	spFloodFill(ITEM_SHIELD_RADIUS * 0.75f, ITEM_SHIELD_RADIUS * 0.75f, -1);
	spSelectRenderTarget( spGetWindowSurface() );
}

