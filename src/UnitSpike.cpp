#include "UnitSpike.h"

#include <cmath>
#include "UtilityFunctions.h"

// Pixels per millisecond
#define SPIKE_ATTACK_SPEED 0.1f
#define SPIKE_ATTACK_RADIUS 200.0f

UnitSpike::UnitSpike( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	idle = spNewSprite();
	attack = spNewSprite();
	image = spLoadSurface( "images/units/spike.png" );
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	spNewSubSpriteWithTiling( attack, image, 64, 0, 64, 64, 1000 );
	activeSprite = idle;
	shape.radius = 20;
	//shape.size = Vector2d<float>(36,36);
	x = &(shape.pos.x);
	y = &(shape.pos.y);
}

UnitSpike::~UnitSpike()
{
	spDeleteSprite( idle );
	spDeleteSprite( attack );
	SDL_FreeSurface( image );
}


///--- PUBLIC ------------------------------------------------------------------

void UnitSpike::ai( Uint32 delta, UnitBase *player )
{
	float dist = sqrt( Utility::sqr( *x - *player->x ) + Utility::sqr( *y - *player->y ) );
	if ( dist < SPIKE_ATTACK_RADIUS )
	{

		*x -= (*x - *player->x) / dist * SPIKE_ATTACK_SPEED * delta;
		*y -= (*y - *player->y) / dist * SPIKE_ATTACK_SPEED * delta;
		activeSprite = attack;
		props.addFlag(ufDeadlyOnTouch);
	}
	else
	{
		activeSprite = idle;
		props.removeFlag(ufDeadlyOnTouch);
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
