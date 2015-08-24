#ifndef _BASE_STATE_H
#define _BASE_STATE_H

#include "SDL.h"
#include <string>
#include <vector>

#include "Timer.h"

#define MAX_DELTA 5

class StateBase
{
public:
	StateBase();
	virtual ~StateBase();

	virtual int update( Uint32 delta );
	virtual int pauseUpdate( Uint32 delta );
	virtual void render( SDL_Surface* target )=0;
	virtual void pauseRender( SDL_Surface* target );

	std::string getLastError() const { return errorString; }

	enum StateType
	{
		stNone=0,
		stLevel,
		stCollision,
		stScore,
		stReplay,
		stMenu,
		stHighscores,
		stReplayLoader,
		stError,
		stWave
	};
	StateType type;

	std::string message;

	bool paused;

protected:
	std::string errorString;
	std::vector< Timer* > timers;
private:

};


#endif // _BASE_STATE_H

