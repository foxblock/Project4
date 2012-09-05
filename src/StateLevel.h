#ifndef _LEVEL_STATE_H
#define _LEVEL_STATE_H

#include "StateBase.h"
#include "UnitBase.h"
#include "Events.h"
#include "ScoreNormal.h"
#include "SpawnNormal.h"
#include "Replay.h"
#include "ShapeCircle.h"
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

	void addUnit( UnitBase *newUnit );
	size_t countUnits() const { return units.size(); }
	// This function takes ownership of the event passed!
	void addEvent( EventBase *newEvent );

	PLAYER_CLASS *player;
	ScoreNormal scoreKeeper;
	SpawnNormal spawnHandler;

	Replay *run;
	int timecode;

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
	ScoreNormal::ScoreMode scoreMode;
	SDL_Surface *pauseScreen;
	spFontPointer pauseText;

#ifdef _DEBUG
	spFontPointer debugText;
#endif

private:
};


#endif // _LEVEL_STATE_H

