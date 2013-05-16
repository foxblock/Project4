#ifndef _STATE_WAVE_H
#define _STATE_WAVE_H

#include "StateLevel.h"
#include "SpawnFile.h"

class StateWave : public StateLevel
{
public:
	StateWave( const std::string &levelFilename, const std::string &replayFilename = "" );
	virtual ~StateWave();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

protected:

private:
	SpawnFile __spawnHandler;
};


#endif // _STATE_WAVE_H

