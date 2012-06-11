#include "UnitLaser.h"

#include <cmath>
#include <sstream>

#include "UtilityFunctions.h"
#include "StateLevel.h"

#define EYE_DISTANCE 15.0f
// Radians per millisecond
#define ROTATION_SPEED 0.01f
#define ROTATION_THESHOLD 0.05f
#define CHARGE_TIME 500
// Actual value: 300
#define LASER_ATTACK_RADIUS_SQR 90000.0f

UnitLaser::UnitLaser( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	image = spLoadSurface( "images/units/laser.png" );
	idle = spNewSprite();
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	activeSprite = idle;

	rotation = 0;
	//shape.size = Vector2d<float>( 55, 55 );
	shape.radius = 32;
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	projectile = NULL;
	hasCharged = false;
}

UnitLaser::~UnitLaser()
{
	spDeleteSprite( idle );
	SDL_FreeSurface( image );
	if ( projectile )
		projectile->toBeRemoved = true;
}


///--- PUBLIC ------------------------------------------------------------------

int UnitLaser::update( Uint32 delta )
{

	if ( !projectile && charge.getStatus() == -1 && hasCharged )
	{
		if ( parent )
		{
			projectile = new ProjectileLaser( parent, 500 );
			parent->addUnit( projectile );
		}
		hasCharged = false;
	}
	if ( projectile )
	{
		if ( projectile->toBeRemoved )
			projectile = NULL;
		else
		{
			Vector2d<float> angle( cos( rotation ), sin( rotation ) );
			projectile->shape.pos = shape.pos + angle * EYE_DISTANCE;
			projectile->shape.target = shape.pos + angle * 1000;
		}
	}
	return UnitBase::update( delta );
}

void UnitLaser::render( SDL_Surface *target )
{
	UnitBase::render( target );

	if ( hasCharged )
	{
		float factor = (float)charge.getTime() / (float)CHARGE_TIME;
		spEllipse( *x + cos( rotation ) * EYE_DISTANCE, *y + sin( rotation ) *
					EYE_DISTANCE, -1, 4, 4, SDL_MapRGB( target->format, 255.0f * (1 - factor), 0, 255.0f * factor ) );
	}
	else
		spEllipse( *x + cos( rotation ) * EYE_DISTANCE, *y + sin( rotation ) *
					EYE_DISTANCE, -1, 4, 4, SDL_MapRGB( target->format, 0, 0, 255 ) );
}

bool UnitLaser::checkCollision( UnitBase const *const other ) const
{
	if ( other != projectile && shape.checkCollision( other->shape ) )
		return true;
	return false;
}

void UnitLaser::ai( Uint32 delta, UnitBase *player )
{
	float diffX = *player->x - *x;
	float diffY = *y - *player->y;
	if ( !hasCharged && (Utility::sqr(diffX) + Utility::sqr(diffY) < LASER_ATTACK_RADIUS_SQR || projectile))
	{
		float newRot = 0;
		if ( diffY > 0 )
			newRot = ( -M_PI_2 + atan( diffX / diffY ) );
		else if ( diffY < 0 )
			newRot = ( M_PI_2 + atan( diffX / diffY ) );

		if ( newRot < -M_PI_2 && rotation > M_PI_2 )
			newRot += 2 * M_PI;
		else if ( newRot > M_PI_2 && rotation < -M_PI_2 )
			newRot -= 2 * M_PI;

		if ( !projectile && fabs( newRot - rotation ) < ROTATION_THESHOLD )
		{
			//rotation = newRot;
			charge.start( CHARGE_TIME );
			hasCharged = true;
		}
		rotation += ( newRot - rotation ) / ( 2 * M_PI ) * ROTATION_SPEED * delta;

		if ( rotation < -M_PI )
			rotation += 2 * M_PI;
		else if ( rotation > M_PI )
			rotation -= 2 * M_PI;
	}
#ifdef _DEBUG
	debugString = Utility::numToStr( rotation );
#endif
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
