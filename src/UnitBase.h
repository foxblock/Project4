#ifndef _UNITBASE_H
#define _UNITBASE_H

#include "Vector2d.h"
#include "SimpleFlags.h"

#include "sparrowSprite.h"
#include "sparrowFont.h"
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

	virtual bool checkCollision( UnitBase const *const other );
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
		ufFrozen = 16,
		ufBackground = 32,
		ufSolid = 64,
		ufReflective = 128,
		ufEOL = 256, // this marks the end of the unit flag list
	};
	enum UnitType
	{
		utNone = 0,
		utPlayer,
		utSpike,
		utLaser,
		utBomb,
		utEOL, // end of unit types
		utItemVortex,
		utItemShield,
		utItemEOL, // end of item types
		utText,
		utBackgroundEOL, // end of background types
		utItemSlowmo, // Moved here to disable, since introduction of slowdown as an ability
	};
	UnitType type;

	float *x;
	float *y;
	Vector2d<float> vel;
	Vector2d<float> accel;
	ShapeBase *shape;
	float mass;

	float maxVel;
	float maxAccel;
	float friction;

#ifdef _DEBUG
	std::string debugString;
	static spFontPointer debugFont;
#endif
protected:
	SDL_Surface *activeSprite;

	StateLevel *parent;

	std::vector< Timer* > timers;
private:
};


#endif // _UNITBASE_H

