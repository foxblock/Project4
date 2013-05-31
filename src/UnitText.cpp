#include "UnitText.h"

#include "sparrowFont.h"
#include "gameDefines.h"
#include "UtilityFunctions.h"

UnitText::UnitText(StateLevel* newParent) :
	UnitBase( newParent, NULL ),
	mode(0),
	fontSize(24),
	colour1(0),
	colour2(0),
	alpha1(255),
	alpha2(255),
	fadeTime1(0),
	fadeTime2(0),
	life(0),
	alignment(0),
	font1( NULL ),
	font2( NULL ),
	fadeMode( 0 )
{
	x = new float(0);
	y = new float(0);
	type = utText;
	timers.push_back( &fadeTimer );
	timers.push_back( &lifeTimer );
	flags.add( ufInvincible );
	flags.add( ufBackground );
}

UnitText::~UnitText()
{
	delete x;
	delete y;
	if ( font1 )
		spFontDelete( font1 );
	if ( font2 )
		spFontDelete( font2 );
}

int UnitText::update(const Uint32& delta)
{
	if ( lifeTimer.isStopped() && life > 0 )
	{
		if ( lifeTimer.wasStarted() )
			toBeRemoved = true;
		else
			lifeTimer.start( life );
	}

	if ( mode == tmBlink && fadeTimer.isStopped() )
	{
		if ( fadeMode == 0 )
		{
			fadeTimer.start( fadeTime2 );
			fadeMode = 1;
		}
		else
		{
			fadeTimer.start( fadeTime1 );
			fadeMode = 0;
		}
	}
	return UnitBase::update( delta );
}

void UnitText::render(SDL_Surface* const target)
{
	if ( !font1 )
	{
		font1 = spFontLoad( FONT_GENERAL, fontSize );
		spFontAdd( font1, SP_FONT_GROUP_ASCII, colour1 );
		if ( alpha1 < 255 )
			Utility::setFontAlpha( font1, alpha1 );
	}
	if ( !font2 && mode == tmBlink )
	{
		font2 = spFontLoad( FONT_GENERAL, fontSize );
		spFontAdd( font2, SP_FONT_GROUP_ASCII, colour2 );
		if ( alpha2 < 255 )
			Utility::setFontAlpha( font2, alpha2 );
	}

	spFontPointer font = NULL;
	if ( fadeMode == 0 )
		font = font1;
	else
		font = font2;
	switch (alignment)
	{
	case taLeft:
		spFontDraw( *x, *y, -1, text.c_str(), font );
		break;
	case taCentre:
		spFontDrawMiddle( *x, *y, -1, text.c_str(), font );
		break;
	case taRight:
		spFontDrawRight( *x, *y, -1, text.c_str(), font );
		break;
	default:
		break;
	}
}

