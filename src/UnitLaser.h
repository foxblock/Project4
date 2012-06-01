#ifndef _UNITLASER_H
#define _UNITLASER_H

#include "UnitBase.h"

class UnitLaser : public UnitBase
{
public:
	UnitLaser( StateLevel* newParent );
	virtual ~UnitLaser();

	void render( SDL_Surface *target );

	void ai( UnitBase* player );
protected:

private:
	spSpritePointer idle;
	SDL_Surface* image;

	float rotation;
};


#endif // _UNITLASER_H

