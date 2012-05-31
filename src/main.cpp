#include "gameDefines.h"
#include SPARROW_PATH

#include "Application.h"

Application myApp;

int update( Uint32 delta )
{
	return myApp.update(delta);
}

void render()
{
	myApp.render();
}
// ^^^ This is really stupid... ^^^

int main( int argc, char **argv )
{
	if ( !myApp.showModal(render,update) )
	{
		printf( "Application terminated with error: %s\n", myApp.getLastError().c_str() );
	}
	else
	{
		printf( "Application shut down gracefully.\n" );
	}

	return 0;
}
