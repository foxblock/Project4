#ifndef _UNITLASER_H
#define _UNITLASER_H

#include "UnitBase.h"

#include "ShapeRect.h"
#include "ProjectileLaser.h"

class UnitLaser : public UnitBase
{
public:
	UnitLaser( StateLevel* newParent );
	virtual ~UnitLaser();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	void ai( UnitBase* player );

	ShapeRect shape;
protected:

private:
	spSpritePointer idle;
	SDL_Surface* image;
	ProjectileLaser* projectile;

	float rotation;
};


#endif // _UNITLASER_H

