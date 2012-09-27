#ifndef _ITEM_VORTEX_H
#define _ITEM_VORTEX_H

#include "UnitBase.h"

#include "Timer.h"
#include "ShapeCircle.h"

class ItemVortex : public UnitBase
{
public:
	ItemVortex( StateLevel *newParent );
	virtual ~ItemVortex();

	int update( const Uint32 &delta );
	void render ( SDL_Surface *const target );

	void ai( const Uint32 &delta, UnitBase *const player );
	void collisionResponse( UnitBase *const other );

protected:
	Timer life;
	Timer vortex;
	ShapeCircle shape;

private:
	static void generateIdleImage();

	static SDL_Surface *idle;
	std::vector<UnitBase*> caught;

	int animationOffset;
	int animationSlow;

};



#endif // _ITEM_VORTEX_H

