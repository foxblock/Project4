#ifndef _UNITLASER_H
#define _UNITLASER_H

#include "UnitBase.h"

#include "ShapeCircle.h"
#include "ProjectileLaser.h"

class UnitLaser : public UnitBase
{
public:
	UnitLaser( StateLevel* newParent );
	virtual ~UnitLaser();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	bool checkCollision( UnitBase const * const other ) const;

	void ai( Uint32 delta, UnitBase* player );

	ShapeCircle shape;
protected:

private:
	static void generateIdleImage();

	static SDL_Surface* idle;
	ProjectileLaser* projectile;

	float angle;
	float angleVel;
	bool hasCharged;
	Timer charge;
};


#endif // _UNITLASER_H

