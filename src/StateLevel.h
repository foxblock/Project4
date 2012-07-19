#ifndef _LEVEL_STATE_H
#define _LEVEL_STATE_H

#include "StateBase.h"
#include "UnitBase.h"
#include "Events.h"
#include "ScoreNormal.h"
#include "SpawnNormal.h"

#define PLAYER_CLASS UnitPlayer

class PLAYER_CLASS;

class StateLevel : public StateBase
{
public:
	StateLevel();
	virtual ~StateLevel();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	void addUnit( UnitBase *newUnit );
	size_t countUnits() const { return units.size(); }
	// This function takes ownership of the event passed!
	void addEvent( EventBase *newEvent );

	PLAYER_CLASS *player;
	ScoreNormal scoreKeeper;
	SpawnNormal spawnHandler;

#ifdef _DEBUG
	std::string debugString;
#endif
protected:
	void handleEvents( Uint32 delta );

	std::vector<UnitBase *> units;
	std::vector<UnitBase *> unitQueue;
	std::vector<EventBase *> eventQueue;

	struct colour
	{
		float r,g,b,intensity;
	};
	colour bgcol;
	colour fadecol;
	Timer bgFadeTimer;
	ScoreNormal::ScoreMode scoreMode;

#ifdef _DEBUG
	spFontPointer debugText;
#endif

private:
};


#endif // _LEVEL_STATE_H

