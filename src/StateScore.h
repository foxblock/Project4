#ifndef _STATE_SCORE_H
#define _STATE_SCORE_H

#include "StateBase.h"
#include "sparrowFont.h"

class StateLevel;

class StateScore : public StateBase
{
public:
	StateScore( StateLevel *level );
	virtual ~StateScore();

	int update( Uint32 delta );
	void render( SDL_Surface *target );
protected:
	SDL_Surface *killFrame;
	int score;
	spFontPointer scoreText;
private:

};


#endif // _STATE_SCORE_H

