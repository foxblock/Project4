#ifndef _STATE_MENU_H
#define _STATE_MENU_H

#include "StateBase.h"
#include <vector>
#include <string>
#include "sparrowFont.h"
#include "Timer.h"

class StateMenu : public StateBase
{
public:
	StateMenu();
	virtual ~StateMenu();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

protected:
	static std::size_t choice;
	spFontPointer fontDark;
	spFontPointer fontBright;
	Timer inputLag;

	SDL_Surface *text;
	Timer textTimer;
	int textMode;
	int textIndex;
	std::vector< std::pair< std::string, std::string > > lines;

	struct MenuEntry
	{
		std::string name;
		int targetState;
	};
	std::vector< MenuEntry > entries;
	void addMenuEntry( const std::string &name, const int &targetState );

private:

};


#endif // _STATE_MENU_H

