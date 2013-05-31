#ifndef EVENT_BASE_H
#define EVENT_BASE_H

class UnitBase;

class EventBase
{
public:
	EventBase()
	{
		type = etNone;
	}

	enum EventType
	{
		etNone=0,
		etUnitDeath,
		etUnitSpawn,
		etBombCascade,
		etSlowMotion,
		etScoreMode,
		etEOL
	};
	EventType type;
};

class EventUnitDeath : public EventBase
{
public:
	EventUnitDeath( UnitBase *const unit, UnitBase *const killer )
	{
		this->unit = unit;
		this->killer = killer;
		points = -1;
		type = etUnitDeath;
	}

	UnitBase *unit;
	UnitBase *killer;
	int points; // filled in by scoreKeeper AFTER creation
};

class EventUnitSpawn : public EventBase
{
public:
	EventUnitSpawn( UnitBase *const unit )
	{
		this->unit = unit;
		type = etUnitSpawn;
	}

	UnitBase *unit;
};

class EventBombCascade : public EventUnitDeath
{
public:
	EventBombCascade( UnitBase *unit, UnitBase *killer ) : EventUnitDeath( unit, killer )
	{
		type = etBombCascade;
	}
};

class EventSlowMotion : public EventBase
{
public:
	EventSlowMotion( const int &duration )
	{
		type = etSlowMotion;
		this->duration = duration;
	}

	int duration;
};

class EventScoreModeChange : public EventBase
{
public:
	EventScoreModeChange( const int &newMode, const int &prevMode )
	{
		type = etScoreMode;
		this->newMode = newMode;
		this->prevMode = prevMode;
	}

	int newMode,prevMode;
};

#endif // EVENT_BASE_H
