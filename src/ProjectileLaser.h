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

	bool checkCollision(UnitBase const * const other);

	ShapeRay shape;
	Timer life;
protected:
	Timer blink;
	int blinkStatus;
	// Used for reflections, a pointer to the next part of the ray
	ProjectileLaser *next;
	UnitBase *reflector;

private:

};


#endif // _PROJECTILE_LASER_H

