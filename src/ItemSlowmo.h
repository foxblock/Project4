#ifndef _ITEM_SLOWMO_H
#define _ITEM_SLOWMO_H

#include "UnitBase.h"

#include "Timer.h"
#include "ShapeCircle.h"

class ItemSlowmo : public UnitBase
{
public:
	ItemSlowmo( StateLevel *newParent );
	virtual ~ItemSlowmo();

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



#endif // _ITEM_SLOWMO_H

