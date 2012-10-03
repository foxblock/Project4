#ifndef _UNITSPIKE_H
#define _UNITSPIKE_H

#include "UnitBase.h"
#include "ShapeCircle.h"
#include "Timer.h"
#include "Vector2d.h"

class UnitSpike : public UnitBase
{
public:
	UnitSpike( StateLevel *newParent );
	virtual ~UnitSpike();

	void ai( const Uint32 &delta, UnitBase *const player );

	void render( SDL_Surface *const target );

	ShapeCircle shape;

	int chargeState; // -1 - out of screen, 0 - idle/chasing, 1 - waiting for charge, 2 - charging
	Vector2d<float> target; // movement target - spike will move to this without thought
protected:

private:
	static void generateIdleImage();
	static void generateAttackImage();

	static SDL_Surface *idle;
	static SDL_Surface *attack;
	Timer chargeTimer;
};


#endif // _UNITSPIKE_H

