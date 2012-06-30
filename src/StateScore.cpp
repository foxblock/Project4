#include "StateScore.h"

#include "StateLevel.h"
#include "gameDefines.h"
#include "sparrowCore.h"
#include "UtilityFunctions.h"

#define SCORE_FONT_SIZE 32

StateScore::StateScore( StateLevel *level ) : StateBase()
{
	SDL_Surface *temp = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );
	killFrame = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );
	spSelectRenderTarget( temp );
	level->render( temp );
	SDL_SetAlpha( temp, SDL_SRCALPHA, 128 );
	SDL_BlitSurface( temp, NULL, killFrame, NULL );
	spSelectRenderTarget( spGetWindowSurface() );
	spDeleteSurface( temp );

	score = level->scoreKeeper.getScore();
	scoreText = spFontLoad( GAME_FONT, SCORE_FONT_SIZE );
	if ( scoreText )
		spFontAddRange( scoreText, ' ', '~', spGetRGB( 255, 255, 255 ) );
	spGetInput()->button[SP_BUTTON_START] = 0;

	type = stScore;
}

StateScore::~StateScore()
{
	spDeleteSurface( killFrame );
}


///--- PUBLIC ------------------------------------------------------------------

int StateScore::update( Uint32 delta )
{
	if ( spGetInput()->button[SP_BUTTON_START] )
		return stLevel;
}

void StateScore::render( SDL_Surface *target )
{
	SDL_BlitSurface( killFrame, NULL, spGetWindowSurface(), NULL );
	if ( scoreText )
	{
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 - SCORE_FONT_SIZE * 2, -1, "You died!", scoreText );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 - SCORE_FONT_SIZE, -1, ("Score:  " + Utility::numToStr( score )).c_str(), scoreText );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 + SCORE_FONT_SIZE, -1, "Press \""SP_BUTTON_START_NAME"\" to go again...", scoreText );
	}

}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
