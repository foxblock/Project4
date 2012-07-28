#ifndef _STATE_HIGHSCORES_H
#define _STATE_HIGHSCORES_H

#include "StateBase.h"

#include "HighscoreFile.h"
#include "sparrowFont.h"
#include "Timer.h"

class StateHighscores : public StateBase
{
public:
	StateHighscores();
	virtual ~StateHighscores();

	int update( Uint32 delta );
	void render( SDL_Surface* target );

protected:
	HighscoreFile file;
	spFontPointer font;
	int offset;
	HighscoreFile::scoreIter offsetIter;
	Timer inputLag;

private:

};


#endif // _STATE_HIGHSCORES_H

