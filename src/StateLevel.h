#ifndef _LEVEL_STATE_H
#define _LEVEL_STATE_H

#include "StateBase.h"
#include "UnitBase.h"
#include "Events.h"
#include "Replay.h"
#include "ShapeCircle.h"
#include "UtilityFunctions.h"
#include "Timer.h"
#include "SpawnNormal.h"
#include "ScoreNormal.h"
#include <vector>
#include <string>

#define PLAYER_CLASS UnitPlayer

class PLAYER_CLASS;

class StateLevel : public StateBase
{
public:
	StateLevel( const std::string &filename = "" );
	virtual ~StateLevel();

	int update( Uint32 delta );
	int pauseUpdate( Uint32 delta );
	void render( SDL_Surface *target );
	void pauseRender( SDL_Surface *target );

	// This function takes ownership of the unit passed!
	void addUnit( UnitBase *newUnit, const bool &generateEvent );
	size_t countUnits() const { return units.size(); }
	// This function takes ownership of the event passed!
	void addEvent( EventBase *newEvent );

	PLAYER_CLASS *player;
	ScoreBase *scoreKeeper;
	SpawnBase *spawnHandler;

	Replay *run;
	int timecode;
	int deltaBkUp;

	bool slowmo;

#ifdef _DEBUG
	std::string debugString;
#endif
protected:
	void handleEvents( Uint32 delta );

	std::vector<UnitBase *> units;
	std::vector<UnitBase *> unitQueue;
	std::vector<EventBase *> eventQueue;

	Utility::colour bgcol;
	std::vector< std::pair<ShapeCircle, Utility::colour> > bgEffects;
	SDL_Surface *pauseScreen;
	spFontPointer pauseText;

	int slowmoCounter;
	Timer slowmoTimer;

#ifdef _DEBUG
	spFontPointer debugText;
#endif

private:
	ScoreNormal __scoreKeeper;
	SpawnNormal __spawnHandler;
};


#endif // _LEVEL_STATE_H

