#ifndef _UNITPLAYER_H
#define _UNITPLAYER_H

#include "UnitBase.h"
#include "ShapeCircle.h"

class UnitPlayer : public UnitBase
{
public:
	UnitPlayer( StateLevel *newParent );
	virtual ~UnitPlayer();

	int update( const Uint32 &delta );
	void render( SDL_Surface *const target );

	ShapeCircle shape;
protected:

private:
	Vector2d<float> lastVel;
};


#endif // _UNITPLAYER_H

