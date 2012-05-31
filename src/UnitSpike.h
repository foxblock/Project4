#ifndef _UNITSPIKE_H
#define _UNITSPIKE_H

#include "UnitBase.h"

class UnitSpike : public UnitBase
{
public:
	UnitSpike( StateLevel *newParent );
	virtual ~UnitSpike();

	void ai( UnitBase* player );
protected:

private:
	spSpritePointer idle;
	spSpritePointer attack;
	SDL_Surface *image;
};


#endif // _UNITSPIKE_H

