#ifndef _UNITPLAYER_H
#define _UNITPLAYER_H

#include "UnitBase.h"
#include "ShapeCircle.h"

class UnitPlayer : public UnitBase
{
public:
	UnitPlayer( StateLevel *newParent );
	virtual ~UnitPlayer();

	int update( Uint32 delta );

	ShapeCircle shape;
protected:

private:
	static void generateIdleImage();

	static SDL_Surface *idle;
};


#endif // _UNITPLAYER_H

