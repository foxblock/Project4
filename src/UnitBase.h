#ifndef _UNITBASE_H
#define _UNITBASE_H

#include "Vector2d.h"
#include "SimpleFlags.h"

#include "sparrowSprite.h"
#include <string>
#include <vector>

// Pixels per millisecond
#define UNIT_DEFAULT_FRICTION 0.005f
#define UNIT_DEFAULT_MAX_VEL 0.5f
#define UNIT_DEFAULT_MAX_ACCEL 0.05f

class ShapeBase;
class StateLevel;
class Timer;

class UnitBase
{
public:
	UnitBase( StateLevel *newParent, ShapeBase *newShape );
	virtual ~UnitBase();

	virtual int update( const Uint32 &delta );
	virtual void render( SDL_Surface *const target );

	virtual bool checkCollision( UnitBase const *const other ) const;
	virtual void collisionResponse( UnitBase *const other );

	virtual void ai( const Uint32 &delta, UnitBase *const player );

	bool toBeRemoved;

	SimpleFlags flags;

	enum UnitFlags
	{
		ufUnknown = 0,
		ufDeadlyOnTouch = 1,
		ufInvincible = 2,
		ufApplySlowmo = 4,
		ufIsPlayer = 8,
		ufEOL = 16
	};
	enum UnitType
	{
		utNone = 0,
		utPlayer,
		utSpike,
		utLaser,
		utBomb,
		utEOL,
		utItemSlowmo,
		utItemVortex,
		utItemEOL
	};
	UnitType type;

	float *x;
	float *y;
	Vector2d<float> vel;
	Vector2d<float> accel;
	ShapeBase *shape;

#ifdef _DEBUG
	std::string debugString;
	static spFontPointer debugFont;
#endif
protected:
	SDL_Surface *activeSprite;

	StateLevel *parent;

	float maxVel;
	float maxAccel;
	float friction;

	std::vector< Timer* > timers;
private:
};


#endif // _UNITBASE_H

