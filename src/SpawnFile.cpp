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
	currentWave(0),
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
			temp.parameter = line.substr( 3 );
			waves.back()->entries.push_back( temp );
		}
		else
		{
			waves.push_back( new SpawnWave() );
			waves.back()->duration = Utility::strToNum<int>( line );
		}
	}
}


int SpawnFile::update(Uint32 delta)
{
	currentTick += delta;
	if ( currentTick >= waves[currentWave]->duration )
	{
		currentTick = 0;
		++currentWave;
		if ( currentWave >= waves.size() )
			currentWave = 0;
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
	std::stringstream str( entry.parameter );
	str >> *(unit->x);
	str >> *(unit->y);
	if ( str.good() )
	{
		switch ( entry.type )
		{
		case UnitBase::utBomb:
			str >> ((UnitBomb*)unit)->pressure;
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
	std::stringstream str( entry.parameter );
	switch ( entry.type )
	{
	case EventBase::etSlowMotion:
	{
		int temp = 0;
		str >> temp;
		event = new EventSlowMotion( temp );
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
	std::stringstream str( entry.parameter );
	switch ( entry.type )
	{
	case ptCircle:
	{
		int type, amount;
		float x, y, radius;
		str >> type;
		str >> x;
		str >> y;
		str >> radius;
		str >> amount;
		if ( x == -1 && y == -1 )
			patternCircle( type, *parent->player->x, *parent->player->y, radius, amount );
		else
			patternCircle( type, x, y, radius, amount );
		break;
	}
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


