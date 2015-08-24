#include "StateHighscores.h"

#include "sparrowCore.h"
#include "sparrowPrimitives.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"

#define HIGHS_FONT_SIZE 36
#define HIGHS_FONT_SIZE_SMALL 24
#define HIGHS_MAX_INPUT_LAG 150
#define HIGHS_MIN_INPUT_LAG 20
#define HIGHS_STEP_INPUT_LAG 20
#define HIGHS_LAG_TIMER 200

const int HIGHS_ENTRIES_ON_SCREEN = ( APP_SCREEN_HEIGHT - 20 ) / HIGHS_FONT_SIZE;

StateHighscores::StateHighscores() :
	StateBase(),
	file( FOLDER_DATA "/" FILE_HIGHSCORE_NORMAL )
{
	fontW = spFontLoad( FONT_GENERAL, HIGHS_FONT_SIZE );
	fontB = spFontLoad( FONT_GENERAL, HIGHS_FONT_SIZE );
	fontHint = spFontLoad( FONT_GENERAL, HIGHS_FONT_SIZE_SMALL );
	if ( fontW )
		spFontAdd( fontW, SP_FONT_GROUP_ALPHABET SP_FONT_GROUP_GERMAN ".:!\"_" SP_FONT_GROUP_NUMBERS, -1 );
	if ( fontB )
		spFontAdd( fontB, SP_FONT_GROUP_ALPHABET SP_FONT_GROUP_GERMAN ".:!\"_" SP_FONT_GROUP_NUMBERS, 0 );
	if ( fontHint )
		spFontAdd( fontHint, SP_FONT_GROUP_ALPHABET SP_FONT_GROUP_GERMAN ".:!\"_" SP_FONT_GROUP_NUMBERS, 0 );

	drawOffset = 0;
	selOffset = 0;
	offsetIter = file.scores.begin();
	selectionIter = file.scores.begin();
	timers.push_back( &inputLag );
	timers.push_back( &inputLagSwitch );
	lagTime = HIGHS_MAX_INPUT_LAG;

	type = stHighscores;
}

StateHighscores::~StateHighscores()
{
	spFontDelete( fontW );
	spFontDelete( fontB );
	spResetAxisState();
	spResetButtonsState();
}


///--- PUBLIC ------------------------------------------------------------------

int StateHighscores::update(Uint32 delta)
{
	StateBase::update( delta );

	if ( inputLag.isStopped() )
	{
		if ( spGetInput()->axis[1] > 0 && selOffset > 0 )
		{
			--selOffset;
			--selectionIter;
			if ( selOffset - drawOffset < HIGHS_ENTRIES_ON_SCREEN / 2 && drawOffset > 0 )
			{
				--drawOffset;
				--offsetIter;
			}
		}
		else if ( spGetInput()->axis[1] < 0 && selOffset < (int)file.scores.size()-1 )
		{
			++selOffset;
			++selectionIter;
			if ( selOffset - drawOffset > HIGHS_ENTRIES_ON_SCREEN / 2 && drawOffset < (int)file.scores.size() - HIGHS_ENTRIES_ON_SCREEN )
			{
				++drawOffset;
				++offsetIter;
			}
		}
		inputLag.start( lagTime );
	}

	if ( spGetInput()->axis[1] == 0 )
	{
		inputLagSwitch.stop();
		inputLag.stop();
	}
	else
	{
		if ( inputLagSwitch.isStopped() )
			inputLagSwitch.start();
		else
			lagTime = std::max( HIGHS_MIN_INPUT_LAG, HIGHS_MAX_INPUT_LAG -
								inputLagSwitch.getTime() / HIGHS_LAG_TIMER * HIGHS_STEP_INPUT_LAG );
	}

	if ( spGetInput()->button[ SP_BUTTON_START ] )
	{
		return stMenu;
	}

	if ( (spGetInput()->button[ SP_BUTTON_B ] ||
		spGetInput()->button[ SP_BUTTON_Y ]) &&
		!file.scores.empty() )
	{
		message = FOLDER_REPLAY "/" + Utility::numToStr( selectionIter->timestamp ) +
						EXTENSION_REPLAY;
		return stReplay;
	}

	return 0;
}

void StateHighscores::render(SDL_Surface* target)
{
	spClearTarget( COLOUR_BACKGROUND );

	int posOffset = spFontWidth( (unsigned char*) Utility::numToStr( file.scores.size() ).c_str(), fontB );
	int numEntries = std::min( HIGHS_ENTRIES_ON_SCREEN, (int)file.scores.size() );
	int vertOffset = ( APP_SCREEN_HEIGHT + 40 - numEntries * HIGHS_FONT_SIZE ) / 2;

	int I = drawOffset;
	for ( HighscoreFile::scoreIter iter = offsetIter; I < drawOffset + numEntries && iter != file.scores.end(); ++iter )
	{
		spFontPointer temp = fontB;
		if ( I == selOffset )
			temp = fontW;
		int yPos = APP_SCREEN_HEIGHT - vertOffset - ( I - drawOffset + 1 ) * HIGHS_FONT_SIZE;
		spFontDrawRight( posOffset + 16, yPos, -1,
						 (unsigned char*) (Utility::numToStr( I + 1 ) + "." ).c_str(), temp );
		spFontDraw( posOffset + 28, yPos, -1,
					(unsigned char*) iter->name.c_str(), temp );
		spFontDrawRight( APP_SCREEN_WIDTH - 16, yPos, -1,
						(unsigned char*) Utility::numToStr( iter->score ).c_str(), temp );
		++I;
	}

	if ( file.scores.empty() )
	{
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2, -1,
						(unsigned char*) "No highscores set yet, go play the game!", fontB );
	}
	else
	{
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT - HIGHS_FONT_SIZE, -1,
						(unsigned char*) "Press \""SP_BUTTON_B_NAME"\" to load the replay for a score. Press \""SP_BUTTON_START_NAME"\" to exit.", fontHint );
	}
}


///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
