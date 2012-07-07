#ifndef _SHAPE_BASE_H
#define _SHAPE_BASE_H

#include "Vector2d.h"
#include <cstdio>
#include "SDL/SDL.h"

class ShapeRect;
class ShapeRay;
class ShapeCircle;

class ShapeBase
{
public:
	ShapeBase() : pos(0,0) { };
	ShapeBase( const Vector2d<float> &newPos ) : pos(newPos) { };
	virtual ~ShapeBase() { };

	virtual bool checkCollision( ShapeBase const * const other ) const = 0;
	virtual bool checkCollision( ShapeRect const * const other ) const = 0;
	virtual bool checkCollision( ShapeRay const * const other ) const = 0;
	virtual bool checkCollision( ShapeCircle const * const other ) const = 0;

	virtual void render( SDL_Surface* target, Uint32 colour ) = 0;

	Vector2d<float> pos;
protected:

private:

};


#endif // _SHAPE_BASE_H

