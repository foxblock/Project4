#ifndef _SPAWN_FILE_ENTRY_H
#define _SPAWN_FILE_ENTRY_H

#include <vector>
#include <string>

class SpawnWave
{
public:
	struct SpawnEntry {
		int entryType;
		int type;
		std::string parameter;
	};
	enum EntryType {
		stNone = 0,
		stUnit,
		stEvent,
		stPattern,
		stText
	};

	int duration;
	std::vector< SpawnEntry > entries;
	bool noSkip;
};

#endif // _SPAWN_FILE_ENTRY_H

