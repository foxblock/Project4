#ifndef _SHAPE_BASE_H
#define _SHAPE_BASE_H

#include "Vector2d.h"
#include <cstdio>

class ShapeRect;
class ShapeRay;

class ShapeBase
{
public:
	ShapeBase() : pos(0,0) { };
	virtual ~ShapeBase() { };

	virtual bool checkCollision( ShapeBase const * const other ) const = 0;
	virtual bool checkCollision( ShapeRect const * const other ) const = 0;
	virtual bool checkCollision( ShapeRay const * const other ) const = 0;

	Vector2d<float> pos;
protected:

private:

};


#endif // _SHAPE_BASE_H

