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
		etBombCascade
	};
	EventType type;
};

class EventUnitDeath : public EventBase
{
public:
	EventUnitDeath( UnitBase *unit, UnitBase *killer )
	{
		this->unit = unit;
		this->killer = killer;
		type = etUnitDeath;
	}

	UnitBase *unit;
	UnitBase *killer;
};

class EventUnitSpawn : public EventBase
{
public:
	EventUnitSpawn( UnitBase *unit )
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

#endif // EVENT_BASE_H
