#ifndef _UNIT_TEXT_H
#define _UNIT_TEXT_H

#include "UnitBase.h"

#include <string>
#include "Timer.h"

class UnitText : public UnitBase
{
public:
	UnitText( StateLevel *newParent );
	virtual ~UnitText();

	virtual int update( const Uint32 &delta );
	virtual void render( SDL_Surface *const target );

	std::string text;
	int mode;
	int fontSize;
	Uint16 colour1;
	Uint16 colour2;
	int fadeTime1;
	int fadeTime2;
	int life;

	enum TextMode
	{
		tmStatic=0,
		tmBlink
	};
protected:
	Timer fadeTimer;
	Timer lifeTimer;
	spFontPointer font1;
	spFontPointer font2;
	int fadeMode;

private:

};


#endif // _UNIT_TEXT_H

