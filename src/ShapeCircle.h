#ifndef _SHAPE_CIRCLE_H
#define _SHAPE_CIRCLE_H

#include "ShapeBase.h"

class ShapeRect;
class ShapeRay;

class ShapeCircle : public ShapeBase
{
public:
	ShapeCircle();
	virtual ~ShapeCircle();

	bool checkCollision( ShapeBase const * const other ) const;
	bool checkCollision( ShapeRect const * const other ) const;
	bool checkCollision( ShapeRay const * const other ) const;
	bool checkCollision( ShapeCircle const * const other ) const;

	void render( SDL_Surface* target, Uint32 colour );

	float radius;

protected:

private:

};


#endif // _SHAPE_CIRCLE_H

