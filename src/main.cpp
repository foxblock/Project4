#include "Application.h"

#include "sparrow3d.h"

Application myApp;

int _loopupdate( Uint32 delta )
{
	return myApp.update(delta);
}

void _looprender()
{
	myApp.render();
}
// ^^^ This is really stupid... ^^^

int main( int argc, char **argv )
{
	if ( !myApp.showModal(_looprender,_loopupdate) )
	{
		printf( "Application terminated with error: %s\n", myApp.getLastError().c_str() );
	}
	else
	{
		printf( "Application shut down gracefully.\n" );
	}

	return 0;
}
