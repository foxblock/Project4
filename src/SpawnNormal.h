#ifndef _SPAWN_NORMAL_H
#define _SPAWN_NORMAL_H

#include "UnitBase.h"
#include "SpawnBase.h"
#include "ShapeBase.h"
#include "Waves.h"
#include <vector>
#include <map>

class SpawnRegion;

class SpawnNormal : public SpawnBase
{
public:
	SpawnNormal( StateLevel *newParent );
	virtual ~SpawnNormal();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	void handleEvent( EventBase const * const event );

	UnitBase * getUnit( const int &type ) const;

protected:
	Vector2d<float> getSpikePosition( ) const;
	Vector2d<float> getLaserPosition( ) const;
	Vector2d<float> getBombPosition( ) const;
	Vector2d<float> getItemPosition( ) const;

	Timer spawnTimer;
	Timer itemTimer;
	std::vector< SpawnRegion* > regions;
	std::vector< WaveBase* > waves;
	int maxUnits;

private:

};

class SpawnRegion
{
public:
	SpawnRegion( std::map< int, int > probMatrix );
	virtual ~SpawnRegion();

	int checkSpawn( UnitBase const *const unit );
	void render( SDL_Surface *target );

	void addRect( const Vector2d<float> &pos, const Vector2d<float> &size );
	void addCircle( const Vector2d<float> &pos, const float &radius );

private:
	std::vector< ShapeBase * > shapes;
	std::map< int, int > probMatrix; // unit type - factor
	int totalCount;
};


#endif // _SPAWN_NORMAL_H

