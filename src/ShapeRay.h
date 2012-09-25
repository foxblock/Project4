#ifndef _SHAPERAY_H
#define _SHAPERAY_H

#include "Vector2d.h"

#include "ShapeBase.h"

class ShapeRect;
class ShapeCircle;

class ShapeRay : public ShapeBase
{
public:
	ShapeRay();
	ShapeRay( const Vector2d<float> &start );
	ShapeRay( const Vector2d<float> &start, const Vector2d<float> &newTarget );
	virtual ~ShapeRay();

	float length() const { return (target - pos).length(); }

	bool checkCollision( ShapeBase const * const other ) const;
	bool checkCollision( ShapeRect const * const other ) const;
	bool checkCollision( ShapeRay const * const other ) const;
	bool checkCollision( ShapeCircle const * const other ) const;

	bool checkCollision( ShapeBase const * const other, CollisionResponse &result ) const;
	bool checkCollision( ShapeRect const * const other, CollisionResponse &result ) const;
	bool checkCollision( ShapeRay const * const other, CollisionResponse &result ) const;
	bool checkCollision( ShapeCircle const * const other, CollisionResponse &result ) const;

	void render( SDL_Surface* target, Uint32 colour );

	Vector2d<float> target;
	bool infiniteLength;
protected:

private:

};


#endif // _SHAPERAY_H

