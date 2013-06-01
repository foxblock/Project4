#include "SpawnFile.h"

#include <fstream>
#include "gameDefines.h"
#include "UtilityFunctions.h"

// Unit classes
#include "UnitPlayer.h"
#include "UnitSpike.h"
#include "UnitLaser.h"
#include "UnitBomb.h"
#include "ItemSlowmo.h"
#include "ItemVortex.h"
#include "UnitText.h"

#include "Events.h"
#include "StateLevel.h"

using namespace Utility;

SpawnFile::SpawnFile(StateLevel* newParent) :
	SpawnBase(newParent),
	finished(false),
	currentTick(0),
	currentWave(-1),
	skipping(false)
{

}

SpawnFile::~SpawnFile()
{
	for ( std::vector< SpawnWave* >::iterator I = waves.begin(); I != waves.end(); ++I )
		delete *I;
}

bool SpawnFile::load(std::fstream& file)
{
	std::string line;
	int dur = -1;
	while ( file.good() )
	{
		getline( file,line );
		if ( line[0] == 0 || line[0] == '#' )
			continue;
		if ( line[0] == '\t' )
		{
			if ( waves.empty() || line.length() < 3 )
				continue;
			SpawnWave::SpawnEntry temp;
			switch ( line[1] )
			{
			case 'u':
				temp.entryType = SpawnWave::stUnit;
				break;
			case 'e':
				temp.entryType = SpawnWave::stEvent;
				break;
			case 'p':
				temp.entryType = SpawnWave::stPattern;
				break;
			case 't':
				temp.entryType = SpawnWave::stText;
				break;
			default:
				temp.entryType = SpawnWave::stNone;
			}
			temp.type = strToNum<int>( line[2] );
			temp.parameter = line.substr( 4 );
			waves.back()->entries.push_back( temp );
		}
		else
		{
			waves.push_back( new SpawnWave() );
			std::vector< std::string > tokens;
			tokenize( line, tokens, " ,\t" );
			waves.back()->duration = strToNum<int>( tokens[0] );
			waves.back()->noSkip = tokens.size() > 1;
		}
	}
	if ( waves.empty() )
		printf( "%s No wave data has been loaded (empty file?)\n", WARNING_STRING );
}


int SpawnFile::update(Uint32 delta)
{
	if ( finished )
		return 1;

	currentTick += delta;

	if ( currentWave >= 0 && !waves[currentWave]->noSkip && parent->countUnits() == 1 && !skipping ) // skip to next wave when one is done
	{
		EventWaveSkip *ev = new EventWaveSkip( waves[currentWave]->duration - currentTick );
		parent->addEvent( ev );
		currentTick = (float)waves[currentWave]->duration - 750;
		skipping = true;
		LOG_MESSAGE( "Skipped to next wave (all units dead)." );
	}

	if ( currentWave < 0 || currentTick >= waves[currentWave]->duration )
	{
		currentTick = 0;
		++currentWave;
		skipping = false;
		if ( currentWave >= waves.size() )
		{
			finished = true;
			LOG_MESSAGE( "Cycled through wave data." );
			return 1;
		}
		for ( std::vector< SpawnWave::SpawnEntry >::iterator I = waves[currentWave]->entries.begin();
				I != waves[currentWave]->entries.end(); ++I )
		{
            switch ( I->entryType )
            {
			case SpawnWave::stUnit:
				parseUnit( *I );
				break;
			case SpawnWave::stEvent:
				parseEvent( *I );
				break;
			case SpawnWave::stPattern:
				parsePattern( *I );
				break;
			case SpawnWave::stText:
				parseText( *I );
				break;
			default:
				break;
            }
		}
	}

	return 0;
}


///


void SpawnFile::parseUnit( const SpawnWave::SpawnEntry &entry )
{
	UnitBase *unit = getUnit( entry.type );
	std::vector< std::string > tokens;
	tokenize( entry.parameter, tokens, " ,\t" );
	if ( !unit || tokens.size() < 2 )
	{
		printf( "%s SpawnFile, failed to parse unit: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}

	if ( tokens[0][0] == 'r' || tokens[0][0] == 'R' )
		*(unit->x) = strToNum<float>( tokens[0].substr(1) ) + *(parent->player->x);
	else
		*(unit->x) = strToNum<float>( tokens[0] );
	if ( tokens[1][0] == 'r' || tokens[1][0] == 'R' )
		*(unit->y) = strToNum<float>( tokens[1].substr(1) ) + *(parent->player->y);
	else
		*(unit->y) = strToNum<float>( tokens[1] );

	if (tokens.size() > 2 )
	{
		switch ( entry.type )
		{
		case UnitBase::utLaser:
			((UnitLaser*)unit)->stationary = true;
			break;
		case UnitBase::utBomb:
			((UnitBomb*)unit)->pressure = strToNum<int>( tokens[2] );
			break;
		default:
			break;
		}
	}

	if ( unit )
		parent->addUnit( unit, true );
}

void SpawnFile::parseEvent( const SpawnWave::SpawnEntry &entry )
{
	EventBase *event = NULL;
	std::vector< std::string > tokens;
	tokenize( entry.parameter, tokens, " ,\t" );
	if ( tokens.empty() )
	{
		printf( "%s SpawnFile, failed to parse event: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}

	switch ( entry.type )
	{
	case EventBase::etSlowMotion:
	{
		event = new EventSlowMotion( strToNum<int>( tokens[0] ) );
		break;
	}
	case EventBase::etScoreMode:
	{
		event = new EventScoreModeChange( strToNum<int>( tokens[0] ), 0 );
		break;
	}
	default:
		break;
	}
	if ( event )
		parent->addEvent( event );
}

void SpawnFile::parsePattern( const SpawnWave::SpawnEntry &entry )
{
	std::vector< std::string > tokens;
	tokenize( entry.parameter, tokens, " ,\t" );
	bool error = false;

	switch ( entry.type )
	{
	case ptCircle: // unitType x y radius amount
	{
		if ( tokens.size() < 5 )
		{
			error = true;
			break;
		}
		float x, y;
		if ( tokens[1][0] == 'r' || tokens[1][0] == 'R' )
			x = strToNum<float>( tokens[1].substr(1) ) + *(parent->player->x);
		else
			x = strToNum<float>( tokens[1] );
		if ( tokens[2][0] == 'r' || tokens[2][0] == 'R' )
			y = strToNum<float>( tokens[2].substr(1) ) + *(parent->player->y);
		else
			y = strToNum<float>( tokens[2] );

		patternCircle( strToNum<int>( tokens[0] ), x, y,
						strToNum<int>( tokens[3] ),
						strToNum<int>( tokens[4] ) );
		break;
	}
	case ptLineH: // unitType amount
		if ( tokens.size() < 2 )
		{
			error = true;
			break;
		}
		patternLineH( strToNum<int>( tokens[0] ),
					strToNum<int>( tokens[1] ) );
		break;
	case ptLineV: // unitType amount
		if ( tokens.size() < 2 )
		{
			error = true;
			break;
		}
		patternLineV( strToNum<int>( tokens[0] ),
					strToNum<int>( tokens[1] ) );
		break;
	default:
		error = true;
	}
	if ( error )
	{
		printf( "%s SpawnFile, failed to parse pattern: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}
}

void SpawnFile::parseText(const SpawnWave::SpawnEntry& entry)
{
	std::vector< std::string > tokens;
	bool error = false;
	UnitText *unit = new UnitText( parent );
	unit->mode = entry.type;
	unit->life = waves[currentWave]->duration;
	int I = 0;

	switch ( entry.type )
	{
	case UnitText::tmStatic: // t0 x y align size r g b text
		tokenize( entry.parameter, tokens, " ,\t", 9 );
		if ( tokens.size() < 9 )
		{
			error = true;
			break;
		}
		*(unit->x) = strToNum<float>( tokens[0] );
		*(unit->y) = strToNum<float>( tokens[1] );
		unit->alignment = strToNum<int>( tokens[2] );
		unit->fontSize = strToNum<float>( tokens[3] );
		unit->colour1 = spGetRGB( strToNum<int>( tokens[4] ),
								strToNum<int>( tokens[5] ),
								strToNum<int>( tokens[6] ) );
		unit->alpha1 = strToNum<int>(tokens[7]);
		unit->text = tokens[8];
		break;
	case UnitText::tmBlink: // t1 x y align size r1 g1 b1 time1 r2 g2 b2 time2 text
		tokenize( entry.parameter, tokens, " ,\t", 15 );
		if ( tokens.size() < 15 )
		{
			error = true;
			break;
		}
		*(unit->x) = strToNum<float>( tokens[0] );
		*(unit->y) = strToNum<float>( tokens[1] );
		unit->alignment = strToNum<int>( tokens[2] );
		unit->fontSize = strToNum<float>( tokens[3] );
		unit->colour1 = spGetRGB( strToNum<int>( tokens[4] ),
								strToNum<int>( tokens[5] ),
								strToNum<int>( tokens[6] ) );
		unit->alpha1 = strToNum<int>(tokens[7]);
		unit->fadeTime1 = strToNum<int>( tokens[8] );
		unit->colour2 = spGetRGB( strToNum<int>( tokens[9] ),
								strToNum<int>( tokens[10] ),
								strToNum<int>( tokens[11] ) );
		unit->alpha2 = strToNum<int>(tokens[12]);
		unit->fadeTime2 = strToNum<int>( tokens[13] );
		unit->text = tokens[14];
		break;
	default:
		error = true;
		delete unit;
	}
	if ( error )
	{
		printf( "%s SpawnFile, failed to parse text: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}

	if ( unit )
		parent->addUnit( unit, false );
}

///

void SpawnFile::patternCircle( const int &type, const float& x, const float& y,
							const float& radius, const int& amount)
{
	for ( int I = 0; I < amount; ++I )
	{
		float angle = 2 * M_PI / amount * I;
		Vector2d<float> pos( x + radius * cos( angle ), y + radius * sin( angle ) );
		UnitBase *unit = getUnit( type );
		*(unit->x) = pos.x;
		*(unit->y) = pos.y;
		parent->addUnit( unit, true );
	}
}

void SpawnFile::patternLineV( const int &type, const int& amount )
{
	for ( int I = 0; I < amount; ++I )
	{
		UnitBase *unit = getUnit( type );
		if ( I < amount / 2 )
			*(unit->x) = APP_SCREEN_WIDTH * 0.1f;
		else
			*(unit->x) = APP_SCREEN_WIDTH * 0.9f;
		*(unit->y) = APP_SCREEN_HEIGHT * 0.1f + APP_SCREEN_HEIGHT * 0.8f * float(I % (amount / 2)) / float(amount / 2 - 1);
		parent->addUnit( unit, true );
	}
}

void SpawnFile::patternLineH( const int &type, const int& amount )
{
	for ( int I = 0; I < amount; ++I )
	{
		UnitBase *unit = getUnit( type );
		if ( I < amount / 2 )
			*(unit->y) = APP_SCREEN_HEIGHT * 0.1f;
		else
			*(unit->y) = APP_SCREEN_HEIGHT * 0.9f;
		*(unit->x) = APP_SCREEN_WIDTH * 0.1f + APP_SCREEN_WIDTH * 0.8f * float(I % (amount / 2)) / float(amount / 2 - 1);
		parent->addUnit( unit, true );
	}
}


