#ifndef _SHAPERAY_H
#define _SHAPERAY_H

#include "Vector2d.h"

#include "ShapeBase.h"

class ShapeRect;

class ShapeRay : public ShapeBase
{
public:
	ShapeRay();
	virtual ~ShapeRay();

	float length() const { return (target - pos).length(); }

	bool checkCollision( ShapeBase const * const other ) const;
	bool checkCollision( ShapeRect const * const other ) const;
	bool checkCollision( ShapeRay const * const other ) const;

	Vector2d<float> target;
	bool infiniteLength;
protected:

private:

};


#endif // _SHAPERAY_H

