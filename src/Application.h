#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <string>

#include "StateBase.h"

class Application
{
public:
	Application();
	virtual ~Application();

	bool showModal( void ( *spDraw )( void ), int ( *spCalc )( Uint32 steps ) );
	std::string getLastError() { return errorString; }

	SDL_Surface *getVideoSurface() { return screen; }

	int update( Uint32 delta );
	void render();
protected:

private:
	SDL_Surface *screen;
	std::string errorString;

	StateBase* activeState;
	StateBase* prevState;
};

#endif // _APPLICATION_H
