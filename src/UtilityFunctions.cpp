#include "UtilityFunctions.h"

namespace Utility {

void tokenize(const std::string& str, std::vector<std::string>& tokens,
			const std::string& delimiters, const int& maxParts)
{
	tokens.clear();
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of( delimiters, 0 );
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of( delimiters, lastPos );

	while ( ( ( maxParts <= 0 ) || tokens.size() < maxParts - 1 ) && ( std::string::npos != pos || std::string::npos != lastPos ) )
	{
		// Found a token, add it to the vector.
		tokens.push_back( str.substr( lastPos, pos - lastPos ) );
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of( delimiters, pos );
		// Find next "non-delimiter"
		pos = str.find_first_of( delimiters, lastPos );
	}
	if ( lastPos < str.size() )
	{
		tokens.push_back( str.substr( lastPos, str.size() - lastPos ) );
	}
}

Uint32 w = 88675123;
Uint32 x = 123456789;
Uint32 y = 362436069;
Uint32 z = 521288629;

void seedRand( const Uint32 &a, const Uint32 &b,
				const Uint32 &c, const Uint32 &d )
{
	w = a;
	x = b;
	y = c;
	z = d;
}

Uint32 rand()
{
	Uint32 t = x ^ ( x << 11 );
	x = y; y = z; z = w;
	w ^= ( w >> 19 ) ^t ^ ( t >> 8 );

	return w;
}

}
