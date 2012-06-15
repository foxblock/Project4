#ifndef _STATE_COLLISION_H
#define _STATE_COLLISION_H

#include "StateBase.h"

#include "sparrowFont.h"

class UnitSpike;
class UnitLaser;
class ProjectileLaser;
class UnitBase;

class StateCollision : public StateBase
{
public:
	StateCollision();
	virtual ~StateCollision();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	void handleInput( Uint32 delta );

	UnitSpike* unit1;
	UnitLaser* unit2;
	ProjectileLaser* unit3;
	ProjectileLaser* unit4;
	UnitBase* player;

	std::string debugString;
	spFontPointer font;
protected:

private:

};


#endif // _STATE_COLLISION_H

