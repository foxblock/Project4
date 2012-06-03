#ifndef _UNITBASE_H
#define _UNITBASE_H

#include "gameDefines.h"

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

	virtual void ai( UnitBase* player );

	bool toBeRemoved;
	bool deadlyOnTouch;
	bool invincible;

	float *x;
	float *y;
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

