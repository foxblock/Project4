#ifndef UNIT_BOMB_H
#define UNIT_BOMB_H

#include "UnitBase.h"
#include "ShapeCircle.h"
#include "Timer.h"

class UnitBomb : public UnitBase
{
public:
	UnitBomb( StateLevel *newParent );
	virtual ~UnitBomb();

	int update( Uint32 delta );
	void render ( SDL_Surface *target );

	void ai( Uint32 delta, UnitBase *player );

	ShapeCircle shape;
protected:

private:
	static void generateIdleImage();
	static void generateFlashingImage();

	static SDL_Surface *idle;
	static SDL_Surface *flashing;

	int pressure;
	int status;
	bool isFlashing;
	Timer flashTimer;
	Timer evadeTimer;
	Timer bombTimer;
};


#endif // UNIT_BOMB_H

