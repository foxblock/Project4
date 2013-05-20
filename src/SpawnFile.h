#ifndef _SPAWN_FILE_H
#define _SPAWN_FILE_H

#include "SpawnBase.h"
#include "SpawnFileEntry.h"

class SpawnFile : public SpawnBase
{
public:
	SpawnFile( StateLevel *newParent );
	virtual ~SpawnFile();

	bool load( std::fstream &file );

	int update( Uint32 delta );
protected:
	void parseUnit( const SpawnWave::SpawnEntry &entry );
	void parseEvent( const SpawnWave::SpawnEntry &entry );
	void parsePattern( const SpawnWave::SpawnEntry &entry );
	void parseText( const SpawnWave::SpawnEntry &entry );

	void patternCircle( const int &type, const float &x, const float &y,
						const float &radius, const int &amount );
	void patternLineH( const int &type, const int &amount );
	void patternLineV( const int &type, const int &amount );

	enum PatternType
	{
		ptNone=0,
		ptCircle,
		ptLineH,
		ptLineV,
		ptEOL
	};

	std::vector< SpawnWave* > waves;
	int currentTick;
	int currentWave;
	bool skipping;
private:

};


#endif // _SPAWN_FILE_H

