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
	Uint8 alpha1;
	Uint8 alpha2;
	int fadeTime1;
	int fadeTime2;
	int life;
	int alignment;

	enum TextAlign
	{
		taLeft=0,
		taCentre,
		taRight
	};

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

