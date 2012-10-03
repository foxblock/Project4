#include "HighscoreFile.h"

#include "UtilityFunctions.h"
#include <vector>

#define FILE_HIGHSCORE_DELIMITER "|"

HighscoreFile::HighscoreFile( const std::string &filename )
{
	// read scores from file
	this->filename = filename;
	file.open( filename.c_str(), std::fstream::in );

	while ( file.is_open() && file.good() )
	{
		std::string line;
		getline( file, line );
		std::vector< std::string > tokens;
		Utility::tokenize( line, tokens, FILE_HIGHSCORE_DELIMITER, SCORE_ENTRY_SIZE );
		if ( tokens.size() < SCORE_ENTRY_SIZE )
			continue;
		ScoreEntry temp = { tokens[0], Utility::strToNum<int>( tokens[1] ),
							Utility::strToNum<time_t>( tokens[2] ), tokens[3] };
		scores.insert( temp );
	}

	file.close();
}

HighscoreFile::~HighscoreFile()
{
	// write scores to file
	file.open( filename.c_str(), std::fstream::out | std::fstream::trunc );

	for ( scoreIter I = scores.begin(); I != scores.end() && file.good(); ++I )
	{
		file << I->name << FILE_HIGHSCORE_DELIMITER <<
				I->score << FILE_HIGHSCORE_DELIMITER <<
				I->timestamp << FILE_HIGHSCORE_DELIMITER <<
				// comment string
				I->comment << "\n";
	}

	file.close();
}


///--- PUBLIC ------------------------------------------------------------------



///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
