#ifndef _SPAWN_NORMAL_H
#define _SPAWN_NORMAL_H

#include "UnitBase.h"
#include "SpawnBase.h"
#include "ShapeBase.h"
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

protected:
	Timer spawnTimer;
	SpawnRegion *corners;
	SpawnRegion *sides;
	SpawnRegion *centre;
	SpawnRegion *screen;

private:

};

class SpawnRegion
{
public:
	SpawnRegion( std::vector< ShapeBase * > shapes, std::map< int, int > probMatrix );
	virtual ~SpawnRegion();

	int checkSpawn( UnitBase const *const unit );
	void render( SDL_Surface *target );

private:
	std::vector< ShapeBase * > shapes;
	std::map< int, int > probMatrix; // unit type - factor
	int totalCount;
};


#endif // _SPAWN_NORMAL_H

