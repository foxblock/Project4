#ifndef _SHAPERECT_H
#define _SHAPERECT_H

#include "Vector2d.h"
#include "SimpleDirection.h"

#include "ShapeBase.h"

class ShapeRay;

class ShapeRect : public ShapeBase
{
public:
	enum RectCorner
	{
		rcTopLeft
	};

public:
	ShapeRect();
	virtual ~ShapeRect();

	Vector2d<float> getCenter() const { return pos; }
	Vector2d<float> getCorner( Directions corner ) const
	{
		switch ( corner )
		{
		case diTOPLEFT:
			return Vector2d<float>(pos.x - size.x / 2.0f, pos.y - size.y / 2.0f );
		case diTOPRIGHT:
			return Vector2d<float>(pos.x + size.x / 2.0f, pos.y - size.y / 2.0f );
		case diBOTTOMLEFT:
			return Vector2d<float>(pos.x - size.x / 2.0f, pos.y + size.y / 2.0f );
		case diBOTTOMRIGHT:
			return Vector2d<float>(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f );
		}
	}

	bool checkCollision( ShapeBase const * const other ) const;
	bool checkCollision( ShapeRect const * const other ) const;
	bool checkCollision( ShapeRay const * const other ) const;

	Vector2d<float> size;
protected:

private:

};


#endif // _SHAPERECT_H

