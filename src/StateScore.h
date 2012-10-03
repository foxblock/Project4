#ifndef _STATE_SCORE_H
#define _STATE_SCORE_H

#include "StateBase.h"
#include "sparrowFont.h"
#include <string>
#include "Timer.h"
#include "HighscoreFile.h"
#include "Replay.h"

#define SCORE_MAX_NAME_LENGTH 20

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
	static char name[SCORE_MAX_NAME_LENGTH];
	int state;
	bool caret;
	Timer caretTimer;
	HighscoreFile file;
	Replay *run;
private:

};


#endif // _STATE_SCORE_H

