#include "Application.h"

#include "sparrow3d.h"
#ifdef _DEBUG
#include "UtilityFunctions.h"
#include "time.h"
#endif

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
#ifdef _DEBUG
	Utility::seedRand( 987654321 );
	printf("Random test: %i %i %i %i\n",Utility::randomRange(-100,100),Utility::randomRange(-100,100),Utility::randomRange(-100,100),Utility::randomRange(-100,100));
	printf("Control group: %i %i %i %i\n",-3, -4, 37, -81);
#endif

	if ( !myApp.showModal(_looprender,_loopupdate) )
	{
		printf( "Application terminated with error: %s\n", myApp.getLastError().c_str() );
	}
	else
	{
		printf( "Application shut down gracefully.\n" );
	}
	fflush(stdout);

	return 0;
}
