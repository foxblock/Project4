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

#include "Events.h"
#include "StateLevel.h"

SpawnFile::SpawnFile(StateLevel* newParent) :
	SpawnBase(newParent),
	currentWave(-1),
	currentTick(0)
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
		if ( line[0] == 0 )
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
			temp.type = Utility::strToNum<int>( line[2] );
			temp.parameter = line.substr( 4 );
			waves.back()->entries.push_back( temp );
		}
		else
		{
			waves.push_back( new SpawnWave() );
			waves.back()->duration = Utility::strToNum<int>( line );
		}
	}
	if ( waves.empty() )
		printf( "%s No wave data has been loaded (empty file?)\n", WARNING_STRING );
}


int SpawnFile::update(Uint32 delta)
{
	currentTick += delta;
	if ( currentWave < 0 || currentTick >= waves[currentWave]->duration )
	{
		currentTick = 0;
		++currentWave;
		if ( currentWave >= waves.size() )
		{
			currentWave = 0;
			LOG_MESSAGE( "Cycled through wave data, starting from beginning." );
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
}


///


void SpawnFile::parseUnit( const SpawnWave::SpawnEntry &entry )
{
	UnitBase *unit = getUnit( entry.type );
	std::vector< std::string > tokens;
	Utility::tokenize( entry.parameter, tokens, " ," );
	if ( !unit || tokens.size() < 2 )
	{
		printf( "%s SpawnFile, failed to parse unit: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}

	if ( tokens[0][0] == 'r' || tokens[0][0] == 'R' )
		*(unit->x) = Utility::strToNum<float>( tokens[0].substr(1) ) + *(parent->player->x);
	else
		*(unit->x) = Utility::strToNum<float>( tokens[0] );
	if ( tokens[1][0] == 'r' || tokens[1][0] == 'R' )
		*(unit->y) = Utility::strToNum<float>( tokens[1].substr(1) ) + *(parent->player->y);
	else
		*(unit->y) = Utility::strToNum<float>( tokens[1] );

	if (tokens.size() > 2 )
	{
		switch ( entry.type )
		{
		case UnitBase::utBomb:
			((UnitBomb*)unit)->pressure = Utility::strToNum<int>( tokens[2] );
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
	Utility::tokenize( entry.parameter, tokens, " ," );
	if ( tokens.empty() )
	{
		printf( "%s SpawnFile, failed to parse event: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}

	switch ( entry.type )
	{
	case EventBase::etSlowMotion:
	{
		event = new EventSlowMotion( Utility::strToNum<int>( tokens[0] ) );
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
	Utility::tokenize( entry.parameter, tokens, " ," );
	bool error = false;

	switch ( entry.type )
	{
	case ptCircle: // type x y radius amount
	{
		if ( tokens.size() < 5 )
		{
			error = true;
			break;
		}
		float x, y;
		if ( tokens[1][0] == 'r' || tokens[1][0] == 'R' )
			x = Utility::strToNum<float>( tokens[1].substr(1) ) + *(parent->player->x);
		else
			x = Utility::strToNum<float>( tokens[1] );
		if ( tokens[2][0] == 'r' || tokens[2][0] == 'R' )
			y = Utility::strToNum<float>( tokens[2].substr(1) ) + *(parent->player->y);
		else
			y = Utility::strToNum<float>( tokens[2] );

		patternCircle( Utility::strToNum<int>( tokens[0] ), x, y,
						Utility::strToNum<int>( tokens[3] ),
						Utility::strToNum<int>( tokens[4] ) );
		break;
	}
	case ptLineH: // type amount
	{
		if ( tokens.size() < 2 )
		{
			error = true;
			break;
		}
		patternLineH( Utility::strToNum<int>( tokens[0] ),
					Utility::strToNum<int>( tokens[1] ) );
		break;
	}
	case ptLineV: // type amount
	{
		if ( tokens.size() < 2 )
		{
			error = true;
			break;
		}
		patternLineV( Utility::strToNum<int>( tokens[0] ),
					Utility::strToNum<int>( tokens[1] ) );
		break;
	}
	default:
		error = true;
		break;
	}
	if ( error )
	{
		printf( "%s SpawnFile, failed to parse pattern: %i %s\n", WARNING_STRING, entry.type, entry.parameter.c_str() );
		return;
	}
}

void SpawnFile::parseText(const SpawnWave::SpawnEntry& entry)
{
		// TODO: this
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

void SpawnFile::patternLineH( const int &type, const int& amount )
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

void SpawnFile::patternLineV( const int &type, const int& amount )
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


