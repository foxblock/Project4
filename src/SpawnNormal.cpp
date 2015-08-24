#include "SpawnNormal.h"

#include "Events.h"
#include "gameDefines.h"
#include "StateLevel.h"
#include "ScoreNormal.h"
#include "UtilityFunctions.h"

#include "UnitBase.h"
#include "UnitPlayer.h"

#include "ShapeRect.h"
#include "ShapeCircle.h"

// Actual value: 200
#define SPAWN_PLAYER_SAFE_RADIUS_SQR 30000.0f

#define UNIT_TYPE_COUNT 2

#define SPAWN_CORNER_WIDTH 200.0f
#define SPAWN_CORNER_HEIGHT 100.0f
#define SPAWN_CENTRE_RADIUS 100.0f
#define SPAWN_SIDE_WIDTH 100.0f
#define SPAWN_SIDE_HEIGHT 50.0f
#define SPAWN_SIDE_OFFSET_H 30.0f
#define SPAWN_SIDE_OFFSET_V 15.0f

#define SPAWN_TIME_START 1000
#define SPAWN_MAX_START 20
#define SPAWN_POINTS_PER_UNIT 1000

#define SPAWN_POSITION_MAX_TRIES 100

#define SPAWN_SPIKE_OUTSIDE_CHANCE 25
#define SPAWN_SPIKE_OUTSIDE_RADIUS 50

#define SPAWN_WAVE_MAX_PROBABILITY 2500

#define SPAWN_TIME_ITEM 3000

SpawnNormal::SpawnNormal( StateLevel *newParent ) : SpawnBase( newParent )
{
	maxUnits = SPAWN_MAX_START;
	Vector2d<float> tempVec;
	std::map< int, int > tempMat;
	SpawnRegion *tempRegion;

	// Corners
	tempMat[ UnitBase::utSpike ] = 35;
	tempMat[ UnitBase::utLaser ] = 40;
	tempMat[ UnitBase::utBomb ] = 25;
	tempRegion = new SpawnRegion( tempMat );
	tempVec = Vector2d<float>( SPAWN_CORNER_WIDTH, SPAWN_CORNER_HEIGHT );
	tempRegion->addRect( Vector2d<float>( SPAWN_CORNER_WIDTH / 2.0f, SPAWN_CORNER_HEIGHT / 2.0f ), tempVec );
	tempRegion->addRect( Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_CORNER_WIDTH / 2, SPAWN_CORNER_HEIGHT / 2.0f ), tempVec );
	tempRegion->addRect( Vector2d<float>( SPAWN_CORNER_WIDTH / 2.0f, APP_SCREEN_HEIGHT - SPAWN_CORNER_HEIGHT / 2.0f ), tempVec );
	tempRegion->addRect( Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_CORNER_WIDTH / 2.0f, APP_SCREEN_HEIGHT - SPAWN_CORNER_HEIGHT / 2.0f ), tempVec );
	regions.push_back( tempRegion );
	tempMat.clear();

	// Sides
	tempMat[ UnitBase::utSpike ] = 40;
	tempMat[ UnitBase::utLaser ] = 25;
	tempMat[ UnitBase::utBomb ] = 35;
	tempRegion = new SpawnRegion( tempMat );
	tempVec = Vector2d<float>( SPAWN_SIDE_WIDTH, SPAWN_SIDE_HEIGHT );
	tempRegion->addRect( Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, SPAWN_SIDE_OFFSET_V + SPAWN_SIDE_HEIGHT / 2.0f ), tempVec );
	tempRegion->addRect( Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, APP_SCREEN_HEIGHT - SPAWN_SIDE_OFFSET_V - SPAWN_SIDE_HEIGHT / 2.0f ), tempVec );
	tempVec = Vector2d<float>( SPAWN_SIDE_HEIGHT, SPAWN_SIDE_WIDTH );
	tempRegion->addRect( Vector2d<float>( SPAWN_SIDE_OFFSET_H + SPAWN_SIDE_HEIGHT / 2.0f, APP_SCREEN_HEIGHT / 2.0f ), tempVec );
	tempRegion->addRect( Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_SIDE_OFFSET_H - SPAWN_SIDE_HEIGHT / 2.0f, APP_SCREEN_HEIGHT / 2.0f ), tempVec );
	regions.push_back( tempRegion );
	tempMat.clear();

	// Centre
	tempMat[ UnitBase::utSpike ] = 60;
	tempMat[ UnitBase::utLaser ] = 15;
	tempMat[ UnitBase::utBomb ] = 25;
	tempRegion = new SpawnRegion( tempMat );
	tempRegion->addCircle( Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 ), SPAWN_CENTRE_RADIUS );
	regions.push_back( tempRegion );
	tempMat.clear();

	// Whole screen
	tempMat[ UnitBase::utSpike ] = 50;
	tempMat[ UnitBase::utLaser ] = 30;
	tempMat[ UnitBase::utBomb ] = 20;
	tempRegion = new SpawnRegion( tempMat );
	tempRegion->addRect( Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, APP_SCREEN_HEIGHT / 2.0f ), Vector2d<float>( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT ) );
	regions.push_back( tempRegion );

	// Waves
	waves.push_back( new WaveBomb() );
	waves.push_back( new WaveSpikeStar() );

	timers.push_back( &spawnTimer );
	timers.push_back( &itemTimer );
}

SpawnNormal::~SpawnNormal()
{
	for ( std::vector< SpawnRegion * >::iterator I = regions.begin(); I != regions.end(); ++I )
		delete (*I);
	for ( std::vector< WaveBase * >::iterator I = waves.begin(); I != waves.end(); ++I )
		delete (*I);
}


///--- PUBLIC ------------------------------------------------------------------

int SpawnNormal::update( Uint32 delta )
{
	SpawnBase::update( delta );

	if ( itemTimer.finished() )
	{
		parent->addUnit( getUnit( Utility::randomRange( UnitBase::utEOL + 1, UnitBase::utItemEOL - 1 ) ), false );
		itemTimer.start( SPAWN_TIME_ITEM );
	}

	maxUnits = SPAWN_MAX_START + parent->scoreKeeper->getScore() / SPAWN_POINTS_PER_UNIT;

	if ( parent->countUnits() >= maxUnits )
		return 0;

	for ( std::vector< WaveBase* >::iterator I = waves.begin(); I != waves.end(); ++I )
	{
		if ( (*I)->checkConditions( parent, this, delta ) > SPAWN_WAVE_MAX_PROBABILITY )
		{
			(*I)->spawn( parent, this );
			spawnTimer.start( SPAWN_TIME_START );
			return 0;
		}
	}

	if ( !spawnTimer.stopped() )
		return 0;

	int unitType = UnitBase::utNone;

	for ( std::vector< SpawnRegion* >::iterator I = regions.begin();
			I != regions.end() && unitType == UnitBase::utNone; ++I )
	{
		unitType = (*I)->checkSpawn( parent->player );
	}

	// create the actual unit
	UnitBase* newUnit = getUnit( unitType );
	if ( newUnit )
	{
		spawnTimer.start( SPAWN_TIME_START );
		parent->addUnit( newUnit, true );
	}

	return 0;
}

void SpawnNormal::handleEvent(EventBase const* const event)
{
	switch ( event->type )
	{
	case EventBase::etScoreMode:
		if ( ((EventScoreModeChange*)event)->newMode == ScoreNormal::smPeace )
			itemTimer.start( SPAWN_TIME_ITEM );
		else
			itemTimer.stop();
		break;
	default:
		break;
	}
}

UnitBase * SpawnNormal::getUnit( const int& type ) const
{
	UnitBase *unit = SpawnBase::getUnit( type );

	if ( !unit )
		return NULL;

	switch ( type )
	{
	case UnitBase::utSpike:
		unit->shape->pos = getSpikePosition();
		break;
	case UnitBase::utLaser:
		unit->shape->pos = getLaserPosition();
		break;
	case UnitBase::utBomb:
		unit->shape->pos = getBombPosition();
		break;

	case UnitBase::utItemSlowmo:
	case UnitBase::utItemVortex:
		unit->shape->pos = getItemPosition();
		break;
	default:
		printf( "%s Passed type to SpawnNormal::getUnit is invalid: %i\n", WARNING_STRING, type );
		break;
	}

	return unit;
}

void SpawnNormal::render( SDL_Surface *target )
{
	#ifdef _DEBUG
	for ( std::vector< SpawnRegion * >::iterator I = regions.begin(); I != regions.end(); ++I )
		(*I)->render( target );
	#endif
}

///--- PROTECTED ---------------------------------------------------------------

Vector2d<float> SpawnNormal::getSpikePosition() const
{
	Vector2d<float> result;
	int method = Utility::randomRange( 1, 100 );
	if ( method < SPAWN_SPIKE_OUTSIDE_CHANCE )
	{
		int temp = Utility::randomRange( 1, APP_SCREEN_HEIGHT * 2 + APP_SCREEN_WIDTH * 2 );
		if ( temp > APP_SCREEN_WIDTH * 2 + APP_SCREEN_HEIGHT )
		{
			result.x = 0 - SPAWN_SPIKE_OUTSIDE_RADIUS;
			result.y = temp % ( APP_SCREEN_WIDTH * 2 + APP_SCREEN_HEIGHT );
		}
		else if ( temp > APP_SCREEN_WIDTH + APP_SCREEN_HEIGHT )
		{
			result.x = temp % ( APP_SCREEN_WIDTH + APP_SCREEN_HEIGHT );
			result.y = APP_SCREEN_HEIGHT + SPAWN_SPIKE_OUTSIDE_RADIUS;
		}
		else if ( temp > APP_SCREEN_WIDTH )
		{
			result.x = APP_SCREEN_WIDTH + SPAWN_SPIKE_OUTSIDE_RADIUS;
			result.y = temp % APP_SCREEN_WIDTH;
		}
		else
		{
			result.x = temp;
			result.y = 0 - SPAWN_SPIKE_OUTSIDE_RADIUS;
		}
	}
	else
	{
		int count = 0;
		result.x = Utility::randomRange( -APP_SCREEN_WIDTH * 0.4f, APP_SCREEN_WIDTH * 0.4f );
		if ( result.x < 0 )
			result.x += APP_SCREEN_WIDTH;
		float temp = 0;
		do
		{
			result.y = Utility::randomRange(-APP_SCREEN_HEIGHT * 0.4f, APP_SCREEN_HEIGHT * 0.4f);
			if ( result.y < 0 )
				result.y += APP_SCREEN_HEIGHT;
			temp = Utility::sqr( result.x - *parent->player->x ) +
					Utility::sqr( result.y - * parent->player->y );
			++count;
		}
		while ( temp < SPAWN_PLAYER_SAFE_RADIUS_SQR && count < SPAWN_POSITION_MAX_TRIES );
	}
	return result;
}

Vector2d<float> SpawnNormal::getLaserPosition() const
{
	Vector2d<float> result;
	int count = 0;
	result.x = Utility::randomRange( APP_SCREEN_WIDTH * 0.25f, APP_SCREEN_WIDTH * 0.75f );
	float temp = 0;
	do
	{
		result.y = Utility::randomRange( APP_SCREEN_HEIGHT * 0.25f, APP_SCREEN_HEIGHT * 0.75f );
		temp = Utility::sqr( result.x - *parent->player->x ) +
				Utility::sqr( result.y - * parent->player->y );
		++count;
	}
	while ( temp < SPAWN_PLAYER_SAFE_RADIUS_SQR && count < SPAWN_POSITION_MAX_TRIES );
	return result;
}

Vector2d<float> SpawnNormal::getBombPosition() const
{
	Vector2d<float> result;
	int count = SPAWN_POSITION_MAX_TRIES;
	float temp = 0;
	do
	{
		if ( count == SPAWN_POSITION_MAX_TRIES )
		{
			result.x = Utility::randomRange( APP_SCREEN_WIDTH * 0.25f, APP_SCREEN_WIDTH * 0.75f );
			count = 0;
		}
		result.y = Utility::randomRange( APP_SCREEN_HEIGHT * 0.25f, APP_SCREEN_HEIGHT * 0.75f );
		temp = Utility::sqr( result.x - *parent->player->x ) +
				Utility::sqr( result.y - * parent->player->y );
		++count;
	}
	while ( temp < SPAWN_PLAYER_SAFE_RADIUS_SQR );
	return result;
}

Vector2d<float> SpawnNormal::getItemPosition() const
{
	Vector2d<float> result;
	result.x = Utility::clamp<float>( APP_SCREEN_WIDTH - *parent->player->x, APP_SCREEN_WIDTH * 0.05, APP_SCREEN_WIDTH * 0.95 );
	result.y = Utility::clamp<float>( APP_SCREEN_HEIGHT - *parent->player->y, APP_SCREEN_HEIGHT * 0.05, APP_SCREEN_HEIGHT * 0.95 );
	return result;
}

///--- PRIVATE -----------------------------------------------------------------



///--- SPAWN REGION ------------------------------------------------------------

SpawnRegion::SpawnRegion(std::map< int, int > probMatrix) :
	totalCount(0)
{
	this->probMatrix = probMatrix;
	for (std::map< int, int >::iterator I = this->probMatrix.begin(); I != this->probMatrix.end(); ++I)
		totalCount += I->second;
}

SpawnRegion::~SpawnRegion()
{
	for ( std::vector< ShapeBase* >::iterator I = shapes.begin(); I != shapes.end(); ++ I )
	{
		delete *I;
	}
}

int SpawnRegion::checkSpawn(UnitBase const* const unit)
{
	bool result = false;
	for ( std::vector< ShapeBase * >::iterator I = shapes.begin(); I != shapes.end(); ++I )
	{
		if ( unit->shape->checkCollision( *I ) )
		{
			result = true;
			break;
		}
	}
	if ( result )
	{
		int val = Utility::randomRange( 0, totalCount-1 );
		int temp = 0;
		for ( std::map< int, int >::iterator I = probMatrix.begin(); I != probMatrix.end(); ++I )
		{
			temp += I->second;
			if ( val < temp )
				return I->first;
		}
	}
	return 0;
}

void SpawnRegion::render(SDL_Surface* target)
{
	for( std::vector< ShapeBase* >::iterator I = shapes.begin(); I != shapes.end(); ++I )
		(*I)->render( target, spGetFastRGB(228,0,228) );
}

void SpawnRegion::addCircle(const Vector2d<float>& pos, const float& radius)
{
	shapes.push_back( new ShapeCircle( pos, radius ) );
}

void SpawnRegion::addRect(const Vector2d<float>& pos, const Vector2d<float>& size)
{
	shapes.push_back( new ShapeRect( pos, size ) );
}

