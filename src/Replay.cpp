#include "Replay.h"

#include <fstream>
#include <vector>
#include "Random.h"
#include "UtilityFunctions.h"

#define FILE_REPLAY_DELIMITER "|"
#define FILE_REPLAY_SUB_DELIMITER ","

Replay::Replay()
{
	//
}

Replay::~Replay()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

void Replay::addEntry( Uint32 delta )
{
	ReplayEntry entry;
	entry.delta = delta;
	getReplayButtons( entry.frameInput );
	RANDOM->copyCache( entry.numbers );
	entries.push_back( entry );
}

int Replay::playEntry()
{
	if ( entries.empty() )
	{
		return 0;
	}

	ReplayEntry entry = entries.front();
	entries.pop_front();
	setReplayButtons( entry.frameInput );
	RANDOM->loadCache( entry.numbers );
	return entry.delta;
}

void Replay::loadFromFile(const std::string& filename)
{
	std::fstream file( filename.c_str(), std::fstream::in );

	while ( file.is_open() && file.good() )
	{
		std::string line;
		getline( file, line );
		if ( line[0] == 0 )
			continue;

		std::vector< std::string > tokens;
		Utility::tokenize( line, tokens, FILE_REPLAY_DELIMITER, REPLAY_ENTRY_SIZE );
		if ( tokens.size() < 2 )
			continue;
		ReplayEntry temp;

		temp.delta = Utility::strToNum<int>( tokens[0] );
		stringToButtons( tokens[1], temp.frameInput );
		if ( tokens.size() > 2 )
		{
			std::vector< std::string > numbers;
			Utility::tokenize( tokens[2], numbers, FILE_REPLAY_SUB_DELIMITER );
			for ( std::vector< std::string >::const_iterator I = numbers.begin(); I != numbers.end(); ++I )
			{
				temp.numbers.push_back( Utility::strToNum<int>( *I ) );
			}
		}

		entries.push_back( temp );
	}

	file.close();
}

void Replay::saveToFile(const std::string& filename)
{
	std::fstream file( filename.c_str(), std::fstream::out | std::fstream::trunc );

	for ( std::list< ReplayEntry >::const_iterator I = entries.begin(); I != entries.end() && file.good(); ++I )
	{
		file << I->delta << FILE_REPLAY_DELIMITER
			<< buttonsToString( I->frameInput ) << FILE_REPLAY_DELIMITER;
		for ( std::list< int >::const_iterator K = I->numbers.begin(); K != I->numbers.end(); ++K )
			file << *K << FILE_REPLAY_SUB_DELIMITER;
		file << "\n";
	}

	file.close();
}

///--- PROTECTED ---------------------------------------------------------------

void Replay::getReplayButtons(SspInput& buttons)
{
	buttons.axis[0] = spGetInput()->axis[0];
	buttons.axis[1] = spGetInput()->axis[1];
}

void Replay::setReplayButtons(const SspInput& buttons)
{
	spGetInput()->axis[0] = buttons.axis[0];
	spGetInput()->axis[1] = buttons.axis[1];
}

std::string Replay::buttonsToString(const SspInput& buttons)
{
	std::stringstream result;
	result << (int)buttons.axis[0] << FILE_REPLAY_SUB_DELIMITER <<
			(int)buttons.axis[1];
	return result.str();
}

void Replay::stringToButtons(const std::string& str, SspInput& buttons)
{
	std::vector< std::string > tokens;
	Utility::tokenize( str, tokens, FILE_REPLAY_SUB_DELIMITER );
	if ( tokens.size() < 2 )
		return;
	buttons.axis[0] = Utility::strToNum<int>( tokens[0] );
	buttons.axis[1] = Utility::strToNum<int>( tokens[1] );
}


///--- PRIVATE -----------------------------------------------------------------
