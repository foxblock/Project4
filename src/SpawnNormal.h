#ifndef _SPAWN_NORMAL_H
#define _SPAWN_NORMAL_H

#include "SpawnBase.h"
#include "ShapeRect.h"
#include "ShapeCircle.h"

class SpawnNormal : public SpawnBase
{
public:
	SpawnNormal( StateLevel *newParent );
	virtual ~SpawnNormal();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

protected:
	Timer spawnTimer;
	ShapeRect corner[4];
	ShapeRect side[4];
	ShapeCircle center;

private:

};


#endif // _SPAWN_NORMAL_H

