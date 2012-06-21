#ifndef _UNITBASE_H
#define _UNITBASE_H

#include "Vector2d.h"
#include "SimpleFlags.h"

#include "sparrowSprite.h"
#include <string>

// Pixels per millisecond
#define UNIT_DEFAULT_FRICTION 0.005f
#define UNIT_DEFAULT_MAX_VEL 0.75f
#define UNIT_DEFAULT_MAX_ACCEL 0.1f

class ShapeBase;
class StateLevel;

class UnitBase
{
public:
	UnitBase( StateLevel *newParent, ShapeBase *newShape );
	virtual ~UnitBase();

	virtual int update( Uint32 delta );
	virtual void render( SDL_Surface *target );

	virtual bool checkCollision( UnitBase const * const other ) const;
	virtual void collisionResponse( UnitBase * const other );

	virtual void ai( Uint32 delta, UnitBase* player );

	bool toBeRemoved;

	SimpleFlags props;

	enum UnitFlags {
		ufUNKNOWN=0,
		ufDeadlyOnTouch=1,
		ufInvincible=2,
		ufEOL=4
	};

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
	spSpritePointer activeSprite;

	StateLevel *parent;

	float maxVel;
	float maxAccel;
	float friction;
private:

};


#endif // _UNITBASE_H

