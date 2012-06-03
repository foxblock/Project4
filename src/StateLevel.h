#ifndef _LEVEL_STATE_H
#define _LEVEL_STATE_H

#include <vector>

#include "StateBase.h"
#include "UnitBase.h"
#include "Timer.h"

class PLAYER_CLASS;

class StateLevel : public StateBase
{
public:
	StateLevel();
	virtual ~StateLevel();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	void addUnit(UnitBase *newUnit);

	PLAYER_CLASS *player;

#ifdef _DEBUG
	std::string debugString;
#endif
protected:
	void handleInput();

	std::vector<UnitBase *> units;
	std::vector<UnitBase *> queue;
	Timer spawnTimer;
	spFontPointer debugText;
private:

};


#endif // _LEVEL_STATE_H

