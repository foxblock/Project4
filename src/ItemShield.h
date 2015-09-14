#ifndef _ITEM_SHIELD_H
#define _ITEM_SHIELD_H

#include "UnitBase.h"

#include "Timer.h"
#include "ShapeCircle.h"

class ItemShield : public UnitBase
{
public:
	ItemShield( StateLevel *newParent );
	virtual ~ItemShield();

	int update( const Uint32 &delta );

	void ai( const Uint32 &delta, UnitBase *const player );
	void collisionResponse( UnitBase *const other );

protected:
	Timer life;
	ShapeCircle shape;

private:
	static void generateIdleImage();

	static SDL_Surface *idle;

};



#endif // _ITEM_SHIELD_H

