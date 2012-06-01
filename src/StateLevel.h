#ifndef _LEVEL_STATE_H
#define _LEVEL_STATE_H

#include <vector>

#include "StateBase.h"
#include "UnitBase.h"
#include "Timer.h"

class UnitLaser;

class StateLevel : public StateBase
{
public:
	StateLevel();
	virtual ~StateLevel();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	std::vector<UnitBase *> units;
	UnitBase *player;

#ifdef _DEBUG
	std::string debugString;
#endif
protected:
	void handleInput();
	bool checkForCollision( UnitBase *unitA, UnitBase *unitB );

	Timer spawnTimer;
	spFontPointer debugText;
private:

};


#endif // _LEVEL_STATE_H

