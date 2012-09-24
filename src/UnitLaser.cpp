#include "UnitLaser.h"

#include <cmath>
#include <sstream>

#include "UtilityFunctions.h"
#include "StateLevel.h"
#include "gameDefines.h"

#define LASER_EYE_DISTANCE 15.0f
// Radians per millisecond
#define LASER_ROTATION_SPEED 0.01f
#define LASER_ROTATION_THESHOLD 0.05f
#define LASER_CHARGE_TIME 500
// Actual value: 300
#define LASER_ATTACK_RADIUS_SQR 90000.0f
#define LASER_IDLE_SPEED 2e-6
#define LASER_IDLE_MAX_SPEED 2e-5
#define LASER_RADIUS 32
#define LASER_TIME_MOVEMENT 2500
#define LASER_MAX_MOVEMENT_SPEED 0.04f
#define LASER_MOVEMENT_ACCEL 0.003f
#define LASER_MOVEMENT_FRICTION 0.001f


SDL_Surface* UnitLaser::idle = NULL;

UnitLaser::UnitLaser( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	activeSprite = idle;

	angle = 0;
	angleVel = 0;
	shape.radius = LASER_RADIUS;
	projectile = NULL;
	hasCharged = false;
	maxAccel = 0.01f;
	type = utLaser;
	life.start( LASER_TIME_MOVEMENT );
	maxVel = LASER_MAX_MOVEMENT_SPEED;
	maxAccel = LASER_MOVEMENT_ACCEL;
	friction = LASER_MOVEMENT_FRICTION;
	timers.push_back( &charge );
	timers.push_back( &life );
}

UnitLaser::~UnitLaser()
{
	if ( projectile )
		projectile->toBeRemoved = true;
}


///--- PUBLIC ------------------------------------------------------------------

int UnitLaser::update( const Uint32 &delta )
{
	angle += angleVel * delta;

	// rotation
	if ( angle < -M_PI )
			angle += 2 * M_PI;
		else if ( angle > M_PI )
			angle -= 2 * M_PI;

	if ( !projectile && charge.isStopped() && hasCharged )
	{
		if ( parent )
		{
			projectile = new ProjectileLaser( parent, 500 );
			parent->addUnit( projectile, false );
			LOG_MESSAGE("Laser shooting");
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
			projectile->shape.pos = shape.pos + rot * LASER_EYE_DISTANCE;
			projectile->shape.target = shape.pos + rot * 1000;
		}
	}

	// movement
	if ( life.isStopped() && life.wasStarted() )
	{
		life.stop();
		float angle = Utility::randomRange( 0, 359 ) * M_PI / 180;
		accel.x = cos( angle ) * LASER_MOVEMENT_ACCEL;
		accel.y = sin( angle ) * LASER_MOVEMENT_ACCEL;
	}
	if ( (*x < shape.radius && accel.x < 0) ||
			(*x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
		accel.x *= -1;
	if ( ( *y < shape.radius && accel.y < 0 ) ||
			(*y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
		accel.y *= -1;
	return UnitBase::update( delta );
}

void UnitLaser::render( SDL_Surface *const target )
{
#ifdef _DEBUG
	debugString += Utility::numToStr( angle ) + "\n" + Utility::numToStr( angleVel );
#endif
	UnitBase::render( target );

	Vector2d<float> eyePos( *x + cos( angle ) * LASER_EYE_DISTANCE, *y + sin( angle ) * LASER_EYE_DISTANCE );

	if ( hasCharged )
	{
		float factor = (float)charge.getTime() / (float)LASER_CHARGE_TIME;
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

void UnitLaser::ai( const Uint32 &delta, UnitBase *const player )
{
	float diffX = *player->x - *x;
	float diffY = *y - *player->y;
	// target player
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

		if ( !projectile && fabs( newAngle - angle ) < LASER_ROTATION_THESHOLD )
		{
			charge.start( LASER_CHARGE_TIME );
			hasCharged = true;
			angleVel = 0;
		}
		angleVel = ( newAngle - angle ) / ( 2 * M_PI ) * LASER_ROTATION_SPEED * delta;
	}
	// idle: random rotation
	else if ( !hasCharged )
	{
		angleVel += ( Utility::randomRange( -1, 1 ) ) * LASER_IDLE_SPEED * delta;
		if ( abs(angleVel) > LASER_IDLE_MAX_SPEED )
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
