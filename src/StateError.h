#ifndef _STATE_ERROR_H
#define _STATE_ERROR_H

#include "StateBase.h"

#include "sparrow3d.h"
#include <string>

class StateError : public StateBase
{
public:
	StateError( const std::string &message, const StateType &returnState );
	virtual ~StateError();

	int update( Uint32 delta );
	void render( SDL_Surface *target );
protected:
	spFontPointer font;
	StateType next;
	std::string msg;
private:

};


#endif // _STATE_ERROR_H

