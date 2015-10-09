#ifndef _UNITPLAYER_H
#define _UNITPLAYER_H

#include "UnitBase.h"
#include "ShapeCircle.h"
#include "Timer.h"

class UnitPlayer : public UnitBase
{
public:
	UnitPlayer( StateLevel *newParent );
	virtual ~UnitPlayer();

	int update( const Uint32 &delta );
	void render( SDL_Surface *const target );

	void activeShield(int duration);
	void deactiveShield();

	ShapeCircle shape;
	int abilityPoints;
protected:

private:
	Vector2d<float> lastVel;

	static void generateShieldImage();
	static SDL_Surface *shield;
	Timer shieldTimer;
};


#endif // _UNITPLAYER_H

