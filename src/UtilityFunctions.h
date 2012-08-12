#ifndef _UTILITY_FUNCTIONS_H
#define _UTILITY_FUNCTIONS_H

#include <sstream>
#include <string>
#include <vector>
#include "Vector2d.h"

#define FLOAT_ACCURACY 1e-10
#define MY_RAND_MAX 4294967295
#define VEC_DELIMIT_CHAR ((std::string)",")

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

namespace Utility
{
template <class T>
inline std::string numToStr( const T &number )
{
	std::ostringstream str;
	str << number;
	return str.str();
};

template <class T>
inline T strToNum( const std::string &str )
{
	std::istringstream ss (str);
	T out;
	ss >> out;
	return out;
};

template <class T>
inline std::string vecToStr( const Vector2d<T> &vec )
{
	std::string result = numToStr<T>( vec.x ) + VEC_DELIMIT_CHAR + numToStr<T>( vec.y );
	return result;
}

template <class T>
inline int sign( const T &number )
{
	return ( ( number > 0 ) - ( 0 > number ) );
}

template <class T>
inline T sqr( const T &number )
{
	return number * number;
}

template <class T>
inline T clamp( const T &number, const T &minV, const T &maxV )
{
	return std::min( std::max( minV, number ), maxV );
}

void tokenize( const std::string &str, std::vector<std::string> &tokens,
				const std::string &delimiters, const int &maxParts = -1 );

extern uint32_t w;
extern uint32_t x;
extern uint32_t y;
extern uint32_t z;

void seedRand( const uint32_t &a = 88675123, const uint32_t &b = 123456789,
				const uint32_t &c = 362436069, const uint32_t &d = 521288629);

uint32_t rand();

inline int64_t randomRange( int64_t lower = 0, int64_t upper = MY_RAND_MAX )
{
	if ( upper < lower )
	{
		int64_t temp = lower;
		lower = upper;
		upper = temp;
	}

	if ( lower < 0 && upper < 0 )
		return -( (int64_t)Utility::rand() % ( -lower + upper + 1 ) - upper );
	else
		return (int64_t)Utility::rand() % ( upper - lower + 1 ) + lower;
}

inline bool floatComp( const float &a, const float &b )
{
	return ( abs( a - b ) < FLOAT_ACCURACY );
}

inline bool floatComp( const Vector2d<float> &a, const Vector2d<float> &b )
{
	return ( floatComp(a.x, b.x) && floatComp(a.y, b.y) );
}

};

#endif // _UTILITY_FUNCTIONS_H

