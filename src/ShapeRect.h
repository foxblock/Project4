#ifndef _SHAPERECT_H
#define _SHAPERECT_H

#include "Vector2d.h"
#include "SimpleDirection.h"

#include "ShapeBase.h"

class ShapeRay;
class ShapeCircle;

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
			return pos - size / 2.0f;
		case diTOPRIGHT:
			return Vector2d<float>(pos.x + size.x / 2.0f, pos.y - size.y / 2.0f );
		case diBOTTOMLEFT:
			return Vector2d<float>(pos.x - size.x / 2.0f, pos.y + size.y / 2.0f );
		case diBOTTOMRIGHT:
			return pos + size / 2.0f;
		case diMIDDLE:
			return pos;
		default:
			return Vector2d<float>(0,0);
		}
	}
	float getSide( Directions side ) const
	{
		switch ( side )
		{
		case diLEFT:
			return pos.x - size.x / 2.0f;
		case diRIGHT:
			return pos.x + size.x / 2.0f;
		case diTOP:
			return pos.y - size.y / 2.0f;
		case diBOTTOM:
			return pos.y - size.y / 2.0f;
		default:
			return 0;
		}
	}

	bool checkCollision( ShapeBase const * const other ) const;
	bool checkCollision( ShapeRect const * const other ) const;
	bool checkCollision( ShapeRay const * const other ) const;
	bool checkCollision( ShapeCircle const * const other ) const;

	void render( SDL_Surface* screen, Uint32 colour );

	Vector2d<float> size;
protected:

private:

};


#endif // _SHAPERECT_H

