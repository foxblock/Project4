#ifndef _PROJECTILE_LASER_H
#define _PROJECTILE_LASER_H

#include "UnitBase.h"

#include "ShapeRay.h"
#include "Timer.h"

class ProjectileLaser : public UnitBase
{
public:
	ProjectileLaser( StateLevel *newParent, const int &duration );
	virtual ~ProjectileLaser();

	int update( const Uint32 &delta );
	void render( SDL_Surface *const target );

	ShapeRay shape;
	Timer life;
protected:
	Timer blink;
	int blinkStatus;

private:

};


#endif // _PROJECTILE_LASER_H

