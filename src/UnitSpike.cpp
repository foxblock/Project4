#include "UnitSpike.h"

#include <cmath>

#define SPIKE_ATTACK_SPEED 0.25
#define SPIKE_ATTACK_RADIUS 200

UnitSpike::UnitSpike( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	idle = spNewSprite();
	attack = spNewSprite();
	image = spLoadSurface( "images/units/spike.png" );
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	spNewSubSpriteWithTiling( attack, image, 64, 0, 64, 64, 1000 );
	activeSprite = idle;
	shape.size = Vector2d<float>(36,36);
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

void UnitSpike::ai( UnitBase *player )
{
	float dist = sqrt( pow( *x - *player->x, 2 ) + pow( *y - *player->y, 2 ) );
	if ( dist < SPIKE_ATTACK_RADIUS )
	{
		*x -= (*x - *player->x) / dist * SPIKE_ATTACK_SPEED;
		*y -= (*y - *player->y) / dist * SPIKE_ATTACK_SPEED;
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
