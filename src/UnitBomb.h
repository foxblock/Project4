#ifndef UNIT_BOMB_H
#define UNIT_BOMB_H

#include "UnitBase.h"
#include "ShapeCircle.h"
#include "Timer.h"

#define BOMB_PRESSURE_LEVEL_1 50
#define BOMB_PRESSURE_LEVEL_2 100
#define BOMB_PRESSURE_LEVEL_3 180
#define BOMB_PRESSURE_LEVEL_4 300
#define BOMB_PRESSURE_LEVEL_5 999 // Explode

class UnitBomb : public UnitBase
{
public:
	UnitBomb( StateLevel *newParent );
	virtual ~UnitBomb();

	int update( const Uint32 &delta );
	void render ( SDL_Surface *const target );

	void collisionResponse( UnitBase *const other );

	void ai( const Uint32 &delta, UnitBase *const player );

	ShapeCircle shape;
	Timer bombTimer;
	int pressure;
protected:

private:
	static void generateIdleImage();
	static void generateFlashingImage();

	static SDL_Surface *idle;
	static SDL_Surface *flashing;

	int status;
	bool isFlashing;
	Timer flashTimer;
	Timer evadeTimer;
};


#endif // UNIT_BOMB_H

