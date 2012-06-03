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

	int update( Uint32 delta );
	void render( SDL_Surface *target );

	ShapeRay shape;
	Timer life;
protected:

private:

};


#endif // _PROJECTILE_LASER_H

