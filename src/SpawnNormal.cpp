#include "SpawnNormal.h"

#include "Events.h"
#include "gameDefines.h"
#include "StateLevel.h"
#include "UtilityFunctions.h"

// Unit classes
#include "UnitPlayer.h"
#include "UnitSpike.h"
#include "UnitLaser.h"
#include "UnitBomb.h"

// Actual value: 200
#define SPAWN_PLAYER_SAFE_RADIUS_SQR 40000.0f

#define UNIT_TYPE_COUNT 2

#define SPAWN_CORNER_WIDTH 200.0f
#define SPAWN_CORNER_HEIGHT 100.0f
#define SPAWN_CENTER_RADIUS 100.0f
#define SPAWN_SIDE_WIDTH 100.0f
#define SPAWN_SIDE_HEIGHT 50.0f
#define SPAWN_SIDE_OFFSET_H 30.0f
#define SPAWN_SIDE_OFFSET_V 15.0f

#define SPAWN_TIME_START 1000
#define SPAWN_MAX_START 10

SpawnNormal::SpawnNormal( StateLevel *newParent ) : SpawnBase( newParent )
{
	std::vector< ShapeBase* > tempVec;
	std::map< int, int > tempMat;
	ShapeBase* tempShape;

	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( SPAWN_CORNER_WIDTH / 2, SPAWN_CORNER_HEIGHT / 2 );
	tempVec.push_back( tempShape );
	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_CORNER_WIDTH / 2, SPAWN_CORNER_HEIGHT / 2 );
	tempVec.push_back( tempShape );
	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( SPAWN_CORNER_WIDTH / 2, APP_SCREEN_HEIGHT - SPAWN_CORNER_HEIGHT / 2 );
	tempVec.push_back( tempShape );
	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_CORNER_WIDTH / 2, APP_SCREEN_HEIGHT - SPAWN_CORNER_HEIGHT / 2 );
	tempVec.push_back( tempShape );
	for ( std::vector< ShapeBase* >::iterator I = tempVec.begin(); I != tempVec.end(); ++I )
		((ShapeRect*)(*I))->size = Vector2d<float>( SPAWN_CORNER_WIDTH, SPAWN_CORNER_HEIGHT );
	tempMat[ UnitBase::utSpike ] = 35;
	tempMat[ UnitBase::utLaser ] = 40;
	tempMat[ UnitBase::utBomb ] = 25;
	corners = new SpawnRegion( tempVec, tempMat );
	tempVec.clear();
	tempMat.clear();

	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, SPAWN_SIDE_OFFSET_V + SPAWN_SIDE_HEIGHT / 2.0f );
	((ShapeRect*)tempShape)->size = Vector2d<float>( SPAWN_SIDE_WIDTH, SPAWN_SIDE_HEIGHT );
	tempVec.push_back( tempShape );
	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, APP_SCREEN_HEIGHT - SPAWN_SIDE_OFFSET_V - SPAWN_SIDE_HEIGHT / 2.0f );
	((ShapeRect*)tempShape)->size = Vector2d<float>( SPAWN_SIDE_WIDTH, SPAWN_SIDE_HEIGHT );
	tempVec.push_back( tempShape );
	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( SPAWN_SIDE_OFFSET_H + SPAWN_SIDE_HEIGHT / 2.0f, APP_SCREEN_HEIGHT / 2.0f );
	((ShapeRect*)tempShape)->size = Vector2d<float>( SPAWN_SIDE_HEIGHT, SPAWN_SIDE_WIDTH );
	tempVec.push_back( tempShape );
	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_SIDE_OFFSET_H - SPAWN_SIDE_HEIGHT / 2.0f, APP_SCREEN_HEIGHT / 2.0f );
	((ShapeRect*)tempShape)->size = Vector2d<float>( SPAWN_SIDE_HEIGHT, SPAWN_SIDE_WIDTH );
	tempVec.push_back( tempShape );
	tempMat[ UnitBase::utSpike ] = 40;
	tempMat[ UnitBase::utLaser ] = 25;
	tempMat[ UnitBase::utBomb ] = 35;
	sides = new SpawnRegion( tempVec, tempMat );
	tempVec.clear();
	tempMat.clear();

	tempShape = new ShapeCircle();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 );
	((ShapeCircle*)tempShape)->radius = SPAWN_CENTER_RADIUS;
	tempVec.push_back( tempShape );
	tempMat[ UnitBase::utSpike ] = 60;
	tempMat[ UnitBase::utLaser ] = 15;
	tempMat[ UnitBase::utBomb ] = 25;
	centre = new SpawnRegion( tempVec, tempMat );
	tempVec.clear();
	tempMat.clear();

	tempShape = new ShapeRect();
	tempShape->pos = Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 );
	((ShapeRect*)tempShape)->size = Vector2d<float>( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );
	tempVec.push_back( tempShape );
	tempMat[ UnitBase::utSpike ] = 50;
	tempMat[ UnitBase::utLaser ] = 30;
	tempMat[ UnitBase::utBomb ] = 20;
	screen = new SpawnRegion( tempVec, tempMat );

	timers.push_back( &spawnTimer );
}

SpawnNormal::~SpawnNormal()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int SpawnNormal::update( Uint32 delta )
{
	SpawnBase::update( delta );

	if ( !spawnTimer.isStopped() ||
			parent->countUnits() >= SPAWN_MAX_START )
		return 0;

	UnitBase *newUnit = NULL;
	int unitType = 0;

	unitType = corners->checkSpawn( parent->player );
	if ( unitType == 0 )
		unitType = sides->checkSpawn( parent->player );
	if ( unitType == 0 )
		unitType = centre->checkSpawn( parent->player );
	if ( unitType == 0 )
		unitType = screen->checkSpawn( parent->player );

	if ( unitType == 0 )
		return 0;

	switch ( unitType )
	{
	case UnitBase::utSpike:
		newUnit = new UnitSpike( parent );
		break;
	case UnitBase::utLaser:
		newUnit = new UnitLaser( parent );
		break;
	case UnitBase::utBomb:
		newUnit = new UnitBomb( parent );
		break;
	}

	if ( !newUnit )
		return 0;

	parent->addUnit( newUnit );

	*newUnit->x = rand() % APP_SCREEN_WIDTH;
	float temp = 0;
	do
	{
		*newUnit->y = rand() % APP_SCREEN_HEIGHT;
		temp = Utility::sqr( *newUnit->x - *parent->player->x ) +
				Utility::sqr( *newUnit->y - * parent->player->y );
	}
	while ( temp < SPAWN_PLAYER_SAFE_RADIUS_SQR );

	spawnTimer.start( SPAWN_TIME_START );
	EventUnitSpawn *event = new EventUnitSpawn( newUnit );
	parent->addEvent( event );

	return 0;
}

void SpawnNormal::render( SDL_Surface *target )
{
	corners->render( target );
	sides->render( target );
	centre->render( target );
	screen->render( target );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------



SpawnRegion::SpawnRegion(std::vector< ShapeBase* > shapes, std::map< int, int > probMatrix)
{
	this->shapes = shapes;
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
		int val = rand() % totalCount;
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
