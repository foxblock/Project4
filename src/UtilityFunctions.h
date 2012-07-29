#ifndef _UTILITY_FUNCTIONS_H
#define _UTILITY_FUNCTIONS_H

#include <sstream>
#include <string>
#include <vector>
#include "Vector2d.h"

#define FLOAT_ACCURACY 1e-10
#define VEC_DELIMIT_CHAR ((std::string)",")

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

namespace Utility
{
template <class T>
inline std::string numToStr( T number )
{
	std::stringstream str;
	str << number;
	return str.str();
};

template <class T>
inline T strToNum( std::string str )
{
	std::stringstream ss;
	T out;
	ss << str;
	ss >> out;
	return out;
};

template <class T>
inline std::string vecToStr( Vector2d<T> vec )
{
	std::string result = numToStr( vec.x ) + VEC_DELIMIT_CHAR + numToStr( vec.y );
	return result;
}

template <class T>
inline int sign( T number )
{
	return ( ( number > 0 ) - ( 0 > number ) );
}

template <class T>
inline T sqr( T number )
{
	return number * number;
}

template <class T>
inline T clamp( T number, T minV, T maxV )
{
	return std::min( std::max( minV, number ), maxV );
}

inline void tokenize( const std::string &str,
					  std::vector<std::string> &tokens,
					  const std::string &delimiters,
					  const int &maxParts = -1 )
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

inline int randomRange( int lower = 0, int upper = RAND_MAX )
{
	if ( upper < lower )
	{
		int temp = lower;
		lower = upper;
		upper = temp;
	}

	if ( lower < 0 && upper < 0 )
		return -( rand() % ( -lower + upper + 1 ) - upper );
	else
		return rand() % ( upper - lower + 1 ) + lower;
}
};

#endif // _UTILITY_FUNCTIONS_H

