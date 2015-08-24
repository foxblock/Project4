#include "ScoreNormal.h"

#include "Events.h"
#include "UnitBase.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"
#include "StateLevel.h"

#define SCORE_LASER_POINTS 100
#define SCORE_SPIKE_POINTS 50
#define SCORE_BOMB_POINTS 10
#define SCORE_COMBO_START_TIME 3000
#define SCORE_COMBO_MIN_TIME 1000
#define SCORE_COMBO_KILL_TIME 50
#define SCORE_MULTI_PER_KILL 0.2f
#define SCORE_PEACE_TIMER 1500
#define SCORE_PEACE_POINTS 0.01f
#define SCORE_PEACE_MIN_UNITS 4

#define SCORE_FONT_SIZE 32

ScoreNormal::ScoreNormal( StateLevel *newParent ) :
	ScoreBase( newParent )
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
	mode = smNone;

	scoreText = spFontLoad( FONT_GENERAL, SCORE_FONT_SIZE );
	if ( scoreText )
	{
		spFontAdd( scoreText, SP_FONT_GROUP_NUMBERS, -1 );
		spFontAdd( scoreText, "x", -1 );
	}
	peaceTimer.start( SCORE_PEACE_TIMER );
}

ScoreNormal::~ScoreNormal()
{
	spFontDelete( scoreText );
}


///--- PUBLIC ------------------------------------------------------------------

int ScoreNormal::getScore() const
{
	return points;
}

ScoreNormal::ScoreMode ScoreNormal::getMode() const
{
	return mode;
}

int ScoreNormal::update( Uint32 delta )
{
	ScoreBase::update( delta );

	if ( comboTimer.isStopped() && comboTimer.wasStarted() )
	{
		multiplier = 1;
		streak = 0;
		comboTimer.stop();
		peaceTimer.start( SCORE_PEACE_TIMER );
		if ( mode != smNone )
		{
			EventScoreModeChange *modeEv = new EventScoreModeChange( smNone, mode );
			parent->addEvent( modeEv );
			mode = smNone;
		}
	}
	if ( peaceTimer.isStopped() && peaceTimer.wasStarted() )
	{
		if ( parent->countUnits() >= SCORE_PEACE_MIN_UNITS )
		{
			points += SCORE_PEACE_POINTS * multiplier * delta;

			if ( mode != smPeace )
			{
				EventScoreModeChange *modeEv = new EventScoreModeChange( smPeace, mode );
				parent->addEvent( modeEv );
				mode = smPeace;
			}
		}
	}

	return 0;
}

void ScoreNormal::handleEvent( EventBase const * const event )
{
	switch ( event->type )
	{
	case EventBase::etUnitDeath:
	{
		if ( peaceTimer.wasStarted() )
		{
			peaceTimer.stop();
			multiplier = 1;
		}
		LOG_MESSAGE("Adding points for unit killed");
		std::map<int,int>::iterator p = pointMatrix.find( ((EventUnitDeath*)event)->unit->type );
		if ( p != pointMatrix.end() )
		{
			((EventUnitDeath*)event)->points = p->second * multiplier;
			points += ((EventUnitDeath*)event)->points;
			++kills;
			multiplier += SCORE_MULTI_PER_KILL;
			comboTimer.start( std::max( SCORE_COMBO_START_TIME - SCORE_COMBO_KILL_TIME * streak, SCORE_COMBO_MIN_TIME ) );
			++streak;
		}
		if ( mode != smAggression )
		{
			EventScoreModeChange *modeEv = new EventScoreModeChange( smAggression, mode );
			parent->addEvent( modeEv );
			mode = smAggression;
		}
		break;
	}
	case EventBase::etUnitSpawn:
	{
		if ( peaceTimer.isStopped() && peaceTimer.wasStarted() &&
			parent->countUnits() >= SCORE_PEACE_MIN_UNITS )
		{
			multiplier += 1;
		}
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
			spFontDraw( 0, APP_SCREEN_HEIGHT - SCORE_FONT_SIZE, -1, (unsigned char*) Utility::numToStr( comboTimer.getTime() ).c_str(), scoreText );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT - SCORE_FONT_SIZE, -1, (unsigned char*) (Utility::numToStr( multiplier ) + "x").c_str(), scoreText );
		spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - SCORE_FONT_SIZE, -1, (unsigned char*) Utility::numToStr( getScore() ).c_str(), scoreText );
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
