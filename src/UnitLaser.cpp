#include "UnitLaser.h"

#include <cmath>
#include <sstream>

#include "UtilityFunctions.h"
#include "StateLevel.h"
#include "Random.h"

#define EYE_DISTANCE 15.0f
// Radians per millisecond
#define ROTATION_SPEED 0.01f
#define ROTATION_THESHOLD 0.05f
#define CHARGE_TIME 500
// Actual value: 300
#define LASER_ATTACK_RADIUS_SQR 90000.0f
#define LASER_IDLE_SPEED 0.000002f
#define LASER_IDLE_MAX_SPEED 0.00002f
#define LASER_RADIUS 32

SDL_Surface* UnitLaser::idle = NULL;

UnitLaser::UnitLaser( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	activeSprite = idle;

	angle = 0;
	angleVel = 0;
	shape.radius = LASER_RADIUS;
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	projectile = NULL;
	hasCharged = false;
	maxAccel = 0.01f;
	type = utLaser;
	timers.push_back( &charge );
}

UnitLaser::~UnitLaser()
{
	if ( projectile )
		projectile->toBeRemoved = true;
}


///--- PUBLIC ------------------------------------------------------------------

int UnitLaser::update( Uint32 delta )
{
	angle += angleVel * delta;

	if ( angle < -M_PI )
			angle += 2 * M_PI;
		else if ( angle > M_PI )
			angle -= 2 * M_PI;
#ifdef _DEBUG
	debugString += Utility::numToStr( angle ) + "\n" + Utility::numToStr( angleVel );
#endif

	if ( !projectile && charge.isStopped() && hasCharged )
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
			Vector2d<float> rot( cos( angle ), sin( angle ) );
			projectile->shape.pos = shape.pos + rot * EYE_DISTANCE;
			projectile->shape.target = shape.pos + rot * 1000;
		}
	}
	return UnitBase::update( delta );
}

void UnitLaser::render( SDL_Surface *target )
{
	UnitBase::render( target );

	Vector2d<float> eyePos( *x + cos( angle ) * EYE_DISTANCE, *y + sin( angle ) * EYE_DISTANCE );

	if ( hasCharged )
	{
		float factor = (float)charge.getTime() / (float)CHARGE_TIME;
		spEllipse( eyePos.x, eyePos.y, -1, 8, 8, spGetRGB( 255, 255 * factor, 255 * factor ) );
		spEllipse( eyePos.x, eyePos.y, -1, 4, 4, spGetRGB( 255.0f * (1.0f - factor), 0, 255.0f * factor ) );
	}
	else
	{
		spEllipse( eyePos.x, eyePos.y, -1, 8, 8, -1 );
		spEllipse( eyePos.x, eyePos.y, -1, 4, 4, spGetFastRGB( 0, 0, 255 ) );
	}
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
		float newAngle = 0;
		if ( diffY > 0 )
			newAngle = ( -M_PI_2 + atan( diffX / diffY ) );
		else if ( diffY < 0 )
			newAngle = ( M_PI_2 + atan( diffX / diffY ) );

		if ( newAngle < -M_PI_2 && angle > M_PI_2 )
			newAngle += 2 * M_PI;
		else if ( newAngle > M_PI_2 && angle < -M_PI_2 )
			newAngle -= 2 * M_PI;

		if ( !projectile && fabs( newAngle - angle ) < ROTATION_THESHOLD )
		{
			charge.start( CHARGE_TIME );
			hasCharged = true;
			angleVel = 0;
		}
		angleVel = ( newAngle - angle ) / ( 2 * M_PI ) * ROTATION_SPEED * delta;
	}
	else if ( !hasCharged )
	{
		angleVel += ( RANDOM->getNumber( -1, 1 ) ) * LASER_IDLE_SPEED * delta;
		if ( angleVel > LASER_IDLE_MAX_SPEED )
			angleVel = LASER_IDLE_MAX_SPEED;
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

void UnitLaser::generateIdleImage()
{
	idle = spCreateSurface( LASER_RADIUS * 2, LASER_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	spEllipse( LASER_RADIUS, LASER_RADIUS, -1, LASER_RADIUS, LASER_RADIUS, 0 );
	spSelectRenderTarget( spGetWindowSurface() );
}
