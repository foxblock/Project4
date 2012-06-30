#ifndef _BASE_STATE_H
#define _BASE_STATE_H

#include "SDL/SDL.h"
#include <string>

#define MAX_DELTA 5

class StateBase
{
public:
	StateBase();
	virtual ~StateBase();

	virtual int update( Uint32 delta )=0;
	virtual void render( SDL_Surface* target )=0;

	std::string getLastError() { return errorString; }

	enum StateType
	{
		stNone=0,
		stLevel,
		stCollision,
		stScore
	};
	StateType type;

protected:
	std::string errorString;
private:

};


#endif // _BASE_STATE_H

