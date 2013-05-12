#ifndef _WAVES_BASE_H
#define _WAVES_BASE_H

class StateLevel;
class SpawnBase;

class WaveBase
{
public:
	WaveBase();
	virtual ~WaveBase() { };

	virtual int checkConditions( StateLevel const * const level, SpawnBase const * const spawn, const int &delta )=0;
	virtual void spawn( StateLevel *level, SpawnBase *spawn )=0;

	int probability;
};

class WaveBomb : public WaveBase
{
public:
	int checkConditions( StateLevel const * const level, SpawnBase const * const spawn, const int &delta );
	void spawn( StateLevel *level, SpawnBase *spawn );
};


class WaveSpikeStar : public WaveBase
{
public:
	int checkConditions( StateLevel const * const level, SpawnBase const * const spawn, const int &delta );
	void spawn( StateLevel *level, SpawnBase *spawn );
};



#endif // _WAVES_BASE_H

