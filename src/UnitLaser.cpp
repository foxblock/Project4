#include "UnitLaser.h"

#include <cmath>
#include <sstream>

#include "UtilityFunctions.h"

#define EYE_DISTANCE 15.0f
#define ROTATION_SPEED 0.025f
#define ROTATION_THESHOLD 0.01f

UnitLaser::UnitLaser( StateLevel *newParent ) : UnitBase( newParent )
{
	image = spLoadSurface( "images/units/laser.png" );
	idle = spNewSprite();
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	activeSprite = idle;

	rotation = 0;
	w = 55;
	h = 55;
}

UnitLaser::~UnitLaser()
{
	spDeleteSprite( idle );
	SDL_FreeSurface( image );
}


///--- PUBLIC ------------------------------------------------------------------

void UnitLaser::ai( UnitBase *player )
{
	float diffX = player->x - x;
	float diffY = y - player->y;
	float newRot = 0;
	if ( diffY > 0 )
		newRot = ( -M_PI_2 + atan( diffX / diffY ) );
	else if ( diffY < 0 )
		newRot = ( M_PI_2 + atan( diffX / diffY ) );

	if (newRot < -M_PI_2 && rotation > M_PI_2)
		newRot += 2 * M_PI;
	else if (newRot > M_PI_2 && rotation < -M_PI_2)
		newRot -= 2* M_PI;

	if ( fabs(newRot - rotation) < ROTATION_THESHOLD )
		rotation = newRot;
	else
		rotation += ( newRot - rotation ) / (2 * M_PI) * ROTATION_SPEED;

	if ( rotation < -M_PI )
		rotation += 2 * M_PI;
	else if ( rotation > M_PI )
		rotation -= 2 * M_PI;

#ifdef _DEBUG
	debugString = Utility::numToStr( rotation );
#endif
}

void UnitLaser::render( SDL_Surface *target )
{
	spDrawSprite( x, y, -1, activeSprite );

	spEllipse( x + cos( rotation ) * EYE_DISTANCE, y + sin( rotation ) * EYE_DISTANCE, -1, 4, 4, SDL_MapRGB( target->format, 255, 0, 0 ) );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
