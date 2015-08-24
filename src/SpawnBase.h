#ifndef _SPAWN_BASE_H
#define _SPAWN_BASE_H

#include "SDL.h"
#include "Timer.h"
#include <vector>

class StateLevel;
class UnitBase;
class EventBase;

class SpawnBase
{
public:
	SpawnBase( StateLevel *newUnit );
	virtual ~SpawnBase();

	virtual int update( Uint32 delta );
	virtual void render( SDL_Surface *target );

	virtual void handleEvent( EventBase const * const event );

	virtual UnitBase * getUnit( const int &type ) const;
protected:
	StateLevel *parent;
	std::vector< Timer* > timers;
private:

};


#endif // _SPAWN_BASE_H

