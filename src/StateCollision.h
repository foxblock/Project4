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
	void addUnit( UnitBase *newUnit );

	void handleInput( Uint32 delta );

	UnitBase* player;

	std::string debugString;
	spFontPointer font;

	std::vector<UnitBase *> units;
	std::vector<UnitBase *> unitQueue;
protected:

private:

};


#endif // _STATE_COLLISION_H

