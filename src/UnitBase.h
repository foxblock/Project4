#ifndef _UNITBASE_H
#define _UNITBASE_H

#include "gameDefines.h"
#include "Vector2d.h"
#include "SimpleFlags.h"

#include SPARROW_PATH
#include <string>

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
	ShapeBase *shape;

	#ifdef _DEBUG
	std::string debugString;
	#endif
protected:

	spSpritePointer activeSprite;

	StateLevel *parent;
private:

};


#endif // _UNITBASE_H

