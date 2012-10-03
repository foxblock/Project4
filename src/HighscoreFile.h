#ifndef _HIGHSCORE_FILE_H
#define _HIGHSCORE_FILE_H

#include <string>
#include <set>
#include <fstream>
#include <time.h>
#include "gameDefines.h"

#define SCORE_ENTRY_SIZE 4

class HighscoreFile
{
public:
	struct ScoreEntry
	{
		std::string name;
		int score;
		time_t timestamp;
		std::string comment;
	};
	struct ScoreComp
	{
		bool operator() ( const ScoreEntry &a, const ScoreEntry &b ) const
		{
			if ( a.score != b.score ) // higher scores get placed first
				return ( a.score > b.score );
			else // newer scores get placed first
				return ( a.timestamp > b.timestamp );
		}
	};

public:
	HighscoreFile( const std::string &filename );
	virtual ~HighscoreFile();

	void addScore( const std::string &name, const int &score, const int &timestamp )
	{
		ScoreEntry temp = { name, score, timestamp, VERSION_STRING };
		scores.insert( temp );
	}
	void addScore( const ScoreEntry &entry )
	{
		scores.insert( entry );
	}

	std::multiset< ScoreEntry, ScoreComp > scores;
	typedef std::multiset< ScoreEntry, ScoreComp >::const_iterator scoreIter;

protected:
	std::fstream file;
	std::string filename;

private:

};


#endif // _HIGHSCORE_FILE_H

