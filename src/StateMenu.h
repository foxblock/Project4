#ifndef _STATE_MENU_H
#define _STATE_MENU_H

#include "StateBase.h"
#include <vector>
#include <string>
#include "sparrowFont.h"

class StateMenu : public StateBase
{
public:
	StateMenu();
	virtual ~StateMenu();

	int update( Uint32 delta );
	void render( SDL_Surface *target );

protected:
	unsigned int choice;
	spFontPointer fontDark;
	spFontPointer fontBright;

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

