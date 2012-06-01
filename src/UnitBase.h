#ifndef _UNITBASE_H
#define _UNITBASE_H

#include "gameDefines.h"

#include SPARROW_PATH
#include <string>

class StateLevel;

class UnitBase
{
public:
	UnitBase( StateLevel *newParent );
	virtual ~UnitBase();

	virtual int update( Uint32 delta );
	virtual void render( SDL_Surface *target );

	virtual void ai( UnitBase* player );

	int getWidth() { return w; }
	int getHeight() { return h; }

	float x;
	float y;

	bool toBeRemoved;
	bool deadlyOnTouch;
	bool invincible;

	#ifdef _DEBUG
	std::string debugString;
	#endif
protected:
	int w;
	int h;

	spSpritePointer activeSprite;

	StateLevel *parent;
private:

};


#endif // _UNITBASE_H

