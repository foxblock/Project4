#ifndef _REPLAY_FILE_H
#define _REPLAY_FILE_H

#include <string>
#include <list>
#include "sparrowCore.h"

#define REPLAY_ENTRY_SIZE 2
#define REPLAY_VERSION 5

class Replay
{
public:
	struct ReplayEntry
	{
		Uint32 ms;
		SspInput frameInput;
	};
	struct ReplayInfo
	{
		std::string name;
		int score;
		int timecode;
		int version;
	};

public:
	Replay( );
	virtual ~Replay();

	void update( const bool &forceAdd = false );
	bool play();

	bool loadFromFile( const std::string &filename );
	void saveToFile( const std::string &filename );

	ReplayInfo info;

	bool playing;
	int frameCounter;
	int totalFrames;

	std::string errorString;

protected:
	void getReplayButtons( SspInput &buttons );
	void setReplayButtons( const SspInput &buttons );
	bool compareReplayButtons( const SspInput &a, const SspInput &b );
	std::string buttonsToString( const SspInput &buttons );
	void stringToButtons( const std::string &str, SspInput &buttons );

	std::list< ReplayEntry > entries;
	SspInput lastInput;

private:

};


#endif // _REPLAY_FILE_H


