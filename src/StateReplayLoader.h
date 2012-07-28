#ifndef _STATE_REPLAYLOADER_H
#define _STATE_REPLAYLOADER_H

#include "StateBase.h"

#include "Timer.h"
#include "sparrowFont.h"
#include <string>

class StateReplayLoader : public StateBase
{
public:
	StateReplayLoader();
	virtual ~StateReplayLoader();

	int update( Uint32 delta );
	void render( SDL_Surface* target );

protected:
	spFontPointer font;
	bool caret;
	Timer caretTimer;
	char filenameBuffer[255];

private:

};


#endif // _STATE_REPLAYLOADER_H

