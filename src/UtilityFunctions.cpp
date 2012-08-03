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

uint32_t w = 88675123;
uint32_t x = 123456789;
uint32_t y = 362436069;
uint32_t z = 521288629;

void seedRand( const uint32_t &a, const uint32_t &b,
				const uint32_t &c, const uint32_t &d )
{
	w = a;
	x = b;
	y = c;
	z = d;
}

uint32_t rand()
{
	uint32_t t = x ^ ( x << 11 );
	x = y; y = z; z = w;
	w ^= ( w >> 19 ) ^t ^ ( t >> 8 );

	return w;
}

}
