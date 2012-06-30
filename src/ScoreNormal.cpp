#include "ScoreNormal.h"

#include "Events.h"
#include "UnitBase.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"

#define SCORE_LASER_POINTS 100
#define SCORE_SPIKE_POINTS 50
#define SCORE_BOMB_POINTS 10
#define SCORE_COMBO_START_TIME 2000
#define SCORE_COMBO_MIN_TIME 500
#define SCORE_COMBO_KILL_TIME 50
#define SCORE_MULTI_PER_KILL 0.2f

#define SCORE_FONT_SIZE 32

ScoreNormal::ScoreNormal( StateLevel *newParent ) : ScoreBase( newParent )
{
	points = 0;
	kills = 0;
	multiplier = 1.0f;
	streak = 0;
	timers.push_back( &comboTimer );
	timers.push_back( &peaceTimer );
	pointMatrix[ UnitBase::utSpike ] = SCORE_SPIKE_POINTS;
	pointMatrix[ UnitBase::utLaser ] = SCORE_LASER_POINTS;
	pointMatrix[ UnitBase::utBomb ] = SCORE_BOMB_POINTS;

	scoreText = spFontLoad( GAME_FONT, SCORE_FONT_SIZE );
	if ( scoreText )
	{
		spFontAddRange( scoreText, '0', '9', spGetRGB( 255, 255, 255 ) );
		spFontAdd( scoreText, '.', spGetRGB( 255, 255, 255 ) );
		spFontAdd( scoreText, 'x', spGetRGB( 255, 255, 255 ) );
	}
}

ScoreNormal::~ScoreNormal()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int ScoreNormal::getScore() const
{
	return points;
}

int ScoreNormal::update( Uint32 delta )
{
	if ( comboTimer.isStopped() && comboTimer.wasStarted() )
	{
		multiplier = 1;
		streak = 0;
		comboTimer.stop();
	}
	return ScoreBase::update( delta );
}

void ScoreNormal::handleEvent( EventBase const * const event )
{
	switch ( event->type )
	{
	case EventBase::etUnitDeath:
	{
		std::map<int,int>::iterator p = pointMatrix.find( ((EventUnitDeath*)event)->unit->type);
		if ( p != pointMatrix.end() )
		{
			points += p->second * multiplier;
			++kills;
			multiplier += SCORE_MULTI_PER_KILL;
			comboTimer.start( std::max( SCORE_COMBO_START_TIME - SCORE_COMBO_KILL_TIME * streak, SCORE_COMBO_MIN_TIME ) );
			++streak;
		}
		break;
	}
	default:
		break;
	}
}

void ScoreNormal::render( SDL_Surface *target )
{
	if ( scoreText )
	{
		if ( !comboTimer.isStopped() )
			spFontDraw( 0, APP_SCREEN_HEIGHT - SCORE_FONT_SIZE, -1, Utility::numToStr( comboTimer.getTime() ).c_str(), scoreText );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT - SCORE_FONT_SIZE, -1, (Utility::numToStr( multiplier ) + "x").c_str(), scoreText );
		spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - SCORE_FONT_SIZE, -1, Utility::numToStr( getScore() ).c_str(), scoreText );
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
