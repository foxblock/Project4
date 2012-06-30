#ifndef _SCORE_BASE_H
#define _SCORE_BASE_H

#include "Timer.h"
#include "SDL/SDL.h"
#include <vector>

class ScoreBase;
class EventBase;
class StateLevel;

class ScoreBase
{
public:
	ScoreBase( StateLevel *newParent );
	virtual ~ScoreBase();

	virtual int getScore() const=0;

	virtual int update( Uint32 delta );
	virtual void handleEvent( EventBase const * const event )=0;

	virtual void render( SDL_Surface *target )=0;
protected:
	std::vector< Timer* > timers;
	StateLevel *parent;
private:

};


#endif // _SCORE_BASE_H

