#ifndef _LEVEL_STATE_H
#define _LEVEL_STATE_H

#include <vector>

#include "StateBase.h"
#include "UnitBase.h"

class StateLevel : public StateBase
{
public:
	StateLevel();
	virtual ~StateLevel();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	std::vector<UnitBase *> units;
	UnitBase *player;
protected:
	void handleInput();
	bool checkForCollision( UnitBase *unitA, UnitBase *unitB );
private:

};


#endif // _LEVEL_STATE_H

