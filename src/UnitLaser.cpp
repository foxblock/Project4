#include "UnitLaser.h"

#include <cmath>
#include <sstream>

#include "UtilityFunctions.h"
#include "StateLevel.h"

#define EYE_DISTANCE 15.0f
#define ROTATION_SPEED 0.025f
#define ROTATION_THESHOLD 0.01f

UnitLaser::UnitLaser( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	image = spLoadSurface( "images/units/laser.png" );
	idle = spNewSprite();
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	activeSprite = idle;

	rotation = 0;
	shape.size = Vector2d<float>( 55, 55 );
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	projectile = NULL;
}

UnitLaser::~UnitLaser()
{
	spDeleteSprite( idle );
	SDL_FreeSurface( image );
}


///--- PUBLIC ------------------------------------------------------------------

int UnitLaser::update( Uint32 delta )
{
	if ( projectile )
	{
		if ( projectile->toBeRemoved )
			projectile = NULL;
		else
		{
			Vector2d<float> angle(cos( rotation ), sin( rotation ));
			projectile->shape.pos = shape.pos + angle * EYE_DISTANCE;
			projectile->shape.target = shape.pos + angle * 1000;
		}
	}
	return UnitBase::update( delta );
}

void UnitLaser::render( SDL_Surface *target )
{
	UnitBase::render( target );

	spEllipse( *x + cos( rotation ) * EYE_DISTANCE, *y + sin( rotation ) * EYE_DISTANCE, -1, 4, 4, SDL_MapRGB( target->format, 255, 0, 0 ) );
}

void UnitLaser::ai( UnitBase *player )
{
	float diffX = *player->x - *x;
	float diffY = *y - *player->y;
	float newRot = 0;
	if ( diffY > 0 )
		newRot = ( -M_PI_2 + atan( diffX / diffY ) );
	else if ( diffY < 0 )
		newRot = ( M_PI_2 + atan( diffX / diffY ) );

	if ( newRot < -M_PI_2 && rotation > M_PI_2 )
		newRot += 2 * M_PI;
	else if ( newRot > M_PI_2 && rotation < -M_PI_2 )
		newRot -= 2 * M_PI;

	if ( fabs( newRot - rotation ) < ROTATION_THESHOLD )
	{
		rotation = newRot;
		if ( !projectile )
		{
			projectile = new ProjectileLaser( parent, 500 );
			projectile->shape.pos = shape.pos + Vector2d<float>(cos( rotation ), sin( rotation )) * EYE_DISTANCE;
			parent->addUnit( projectile );
		}
	}
	else
		rotation += ( newRot - rotation ) / ( 2 * M_PI ) * ROTATION_SPEED;

	if ( rotation < -M_PI )
		rotation += 2 * M_PI;
	else if ( rotation > M_PI )
		rotation -= 2 * M_PI;

#ifdef _DEBUG
	debugString = Utility::numToStr( rotation );
#endif
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
