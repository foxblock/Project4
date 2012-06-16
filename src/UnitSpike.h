#ifndef _UNITSPIKE_H
#define _UNITSPIKE_H

#include "UnitBase.h"
#include "ShapeCircle.h"
#include "Timer.h"

class UnitSpike : public UnitBase
{
public:
	UnitSpike( StateLevel *newParent );
	virtual ~UnitSpike();

	void ai( Uint32 delta, UnitBase* player );

	void render( SDL_Surface *target );

	ShapeCircle shape;
protected:

private:
	spSpritePointer idle;
	spSpritePointer attack;
	SDL_Surface *image;
	Timer chargeTimer;
	int chargeState; // 0 - idle/chasing, 1 - waiting for charge, 2 - charging
};


#endif // _UNITSPIKE_H

