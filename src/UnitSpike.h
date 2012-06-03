#ifndef _UNITSPIKE_H
#define _UNITSPIKE_H

#include "UnitBase.h"
#include "ShapeRect.h"

class UnitSpike : public UnitBase
{
public:
	UnitSpike( StateLevel *newParent );
	virtual ~UnitSpike();

	void ai( UnitBase* player );

	ShapeRect shape;
protected:

private:
	spSpritePointer idle;
	spSpritePointer attack;
	SDL_Surface *image;
};


#endif // _UNITSPIKE_H

