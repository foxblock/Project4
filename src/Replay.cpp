#include "Replay.h"

#include <fstream>
#include <vector>
#include "UtilityFunctions.h"

#define FILE_REPLAY_DELIMITER "|"
#define FILE_REPLAY_SUB_DELIMITER ","
#define FILE_REPLAY_INFO_CHAR "#"

Replay::Replay()
{
	frameCounter = 0;
	totalFrames = 0;
	playing = false;
	info.version = REPLAY_VERSION;
	errorString = "";
}

Replay::~Replay()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

void Replay::update( const bool &forceAdd )
{
	++frameCounter;
	if ( entries.empty() )
	{
		ReplayEntry entry;
		getReplayButtons( lastInput );
		entry.frameInput = lastInput;
		entry.ms = frameCounter;
		entries.push_back( entry );
	}
	else
	{
		ReplayEntry entry;
		SspInput temp;
		getReplayButtons( temp );
		if ( forceAdd || !compareReplayButtons( temp, lastInput ) )
		{
			entry.frameInput = temp;
			entry.ms = frameCounter;
			entries.push_back( entry );
			lastInput = temp;
		}
	}
	playing = false;
}

bool Replay::play()
{
	++frameCounter;

	if ( frameCounter == 1 )
	{
		spResetButtonsState();
		spResetAxisState();
		getReplayButtons( lastInput );
	}

	ReplayEntry entry = entries.front();
	if ( frameCounter == entry.ms )
	{
		lastInput = entry.frameInput;
		entries.pop_front();
	}
	setReplayButtons( lastInput );
	playing = true;

	if ( entries.empty() )
	{
		return false;
	}
	return true;
}

bool Replay::loadFromFile(const std::string& filename)
{
	errorString = "";
	std::fstream file( filename.c_str(), std::fstream::in );

	if ( file.good() )
	{
		std::string line;
		getline( file, line );
		if ( line[0] != FILE_REPLAY_INFO_CHAR[0] )
		{
			errorString = "Replay info missing!";
			return false;
		}
		std::vector< std::string > tokens;

		Utility::tokenize( line, tokens, (std::string)FILE_REPLAY_DELIMITER + FILE_REPLAY_INFO_CHAR );
		if ( tokens.size() < 4 )
		{
			errorString = "Replay info malformed!";
			return false;
		}
		info.name = tokens[0];
		info.score = Utility::strToNum<int>( tokens[1] );
		info.timecode = Utility::strToNum<int>( tokens[2] );
		info.version = Utility::strToNum<int>( tokens[3] );

		if ( info.version != REPLAY_VERSION )
		{
			errorString = "File version not matching game version!";
			return false;
		}
	}

	while ( file.is_open() && file.good() )
	{
		std::string line;
		getline( file, line );
		if ( line[0] == 0 )
			continue;

		std::vector< std::string > tokens;
		Utility::tokenize( line, tokens, FILE_REPLAY_DELIMITER, REPLAY_ENTRY_SIZE );
		if ( tokens.size() < REPLAY_ENTRY_SIZE )
			continue;
		ReplayEntry temp;

		temp.ms = Utility::strToNum<int>( tokens[0] );
		stringToButtons( tokens[1], temp.frameInput );

		entries.push_back( temp );
	}

	file.close();
	totalFrames = entries.back().ms;

	playing = !entries.empty();
	if ( !playing )
		errorString = "No replay data was loaded! (empty file?)";
	return playing;
}

void Replay::saveToFile(const std::string& filename)
{
	errorString = "";
	std::fstream file( filename.c_str(), std::fstream::out | std::fstream::trunc );

	if ( file.good() )
	{
		file << FILE_REPLAY_INFO_CHAR << info.name << FILE_REPLAY_DELIMITER <<
				info.score << FILE_REPLAY_DELIMITER <<
				info.timecode << FILE_REPLAY_DELIMITER <<
				info.version << "\n";
	}

	for ( std::list< ReplayEntry >::const_iterator I = entries.begin(); I != entries.end() && file.good(); ++I )
	{
		file << I->ms << FILE_REPLAY_DELIMITER
			<< buttonsToString( I->frameInput ) << "\n";
	}

	if ( !file.good() )
		errorString = "Failed to write replay file!";

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

bool Replay::compareReplayButtons( const SspInput &a, const SspInput &b )
{
	return ( ( a.axis[0] == b.axis[0] ) && ( a.axis[1] == b.axis[1] ) );
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
