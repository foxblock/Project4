#include "StateWave.h"

#include <fstream>

StateWave::StateWave( const std::string &levelFilename, const std::string &replayFilename ) :
	StateLevel( replayFilename ),
	spawnHandler( this )
{
	if ( levelFilename[0] != 0 )
	{
		std::fstream file( levelFilename.c_str(), std::fstream::in );
		std::string line;
		if ( file.good() )
		{
			getline( file,line );
		}
		else
		{
			errorString = "Could not open wave data " + levelFilename;
		}
	}
	else
	{
		errorString = "Wave data missing!";
	}
}

StateWave::~StateWave()
{

}

int StateWave::update(Uint32 delta)
{
	return StateLevel::update(delta);
}

void StateWave::render(SDL_Surface* target)
{
	StateLevel::render(target);
}
